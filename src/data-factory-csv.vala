using GLib;
using Gsf;

private enum ParserState {
  START,
  COLLECT,
  TAILSPACE,
  END_QUOTE
}

public class GGobi.DataFactoryCSV:DataFactory
{ 
  
  private static uint ROW_SIZE_INC = 4;
  
  override SList<string>
  get_supported_modes() 
  {
    SList<string> modes = new SList<string>();
    modes.append("csv");
    return modes;
  }
  
  override SList<string>
  get_base_file_exts_for_mode(string mode) 
  {
    SList<string> exts = null;
    if (mode == null || mode == "csv") {
      exts.prepend("asc".casefold());
      exts.prepend("txt".casefold());
      exts.prepend("csv".casefold());
    }
    return exts;
  }
  
  override SList<Data>
  create_for_input(Input input)
  {
    Input lines;
    Stage d;
    List<double[]> rows = new List<double[]>();
    SList<Data> ds = new SList<Data>();

    /* Open the file */
    lines = new InputTextline(input);

    do { /* parse each row */
      /*cur = ;
      if (cur != null)*/
        rows.prepend(parse_row((InputTextline)lines, ','));
    } while (rows.data != null);

    /* clean up the last bogus entry and reverse */
    rows.remove(rows.data);
    rows.reverse();
    
    /* Load the parsed data into the GGobiStage */
    d = create_data(rows);
    ds.append(d);
    
    // VALABUG: Vala thinks that we don't own 'ds' so it tells us to copy it.
    // VALABUG: Vala fails to correctly free the elements of 'rows'
    return (ds.copy());
  }

/* Heuristic: If the first row has an empty in the first column and
   and all the values in the first column are unique, we have row names.
*/
  private bool has_row_labels (List<SList> rows)
  {
    // VALABUG: always use Hashtable.full and give finalizers - otherwise
    // memory is leaked - this looks like it would be tough to fix
    HashTable<string,string> hash = new HashTable.full<string,string>(str_hash, 
      str_equal, g_free, g_free);
    weak SList<weak string> first = rows.data;
    weak string first_entry = first.data;

    if (first_entry.size() != 0)
      return false;

    foreach(SList<string> row in rows) {
      weak string str = row.data;
      if (hash.lookup(str) != null) {
        warning("Duplicate row name: %s - treating rownames as data", str);
        return false;
      }
      hash.insert(str, str);
    }
    return true;
  }

  private void load_column_labels (SList row, Stage d, bool row_labels)
  {
    uint j = 0;
    SList<string> entries = row_labels ? row : row.next;
    foreach(string entry in entries) {
      if (entry.size() == 0)
        d.set_col_name(j++, null);
      else d.set_col_name(j++, entry);
    }
  }

  private void load_row_labels (List<SList> rows, Stage d, bool has_labels)
  {
    if (!has_labels)
      return;
    int i = 0;
    foreach(SList row in rows)
      d.set_row_id(i++, row.data);
  }

  private void load_row_values (List<SList> rows, Stage d, bool row_labels) {
    uint i = 0;
    foreach(SList row in rows) {
      uint j = 0;
      SList<string> entries = row_labels ? row : row.next;
      foreach(string entry in row)
        d.set_string_value(i++, j++, entry);
    }
  }

  private Stage create_data (List<SList> rows) {
    Data d;
    uint nrows = rows.length(), ncols = 0;

    bool row_labels = has_row_labels(rows);

    /* must have at least 2 CSV rows (first is column names) */
    if (nrows <= 1) {
      critical("CSV file does not contain any data rows; not loading");
      return(null);
    }
  
    ncols = rows.nth_data(0).length();
    if (row_labels)
      ncols--;

    d = new Data(nrows - 1, ncols);
    d.add_attributes();

    load_column_labels(rows.nth_data(0), d, row_labels);
    rows = rows.next;  /* skip the column labels */

    load_row_labels(rows, d, row_labels);
    load_row_values(rows, d, row_labels);

    return (d);
  }

  /* The parse_row function below is based on code from libmba. The GGobi team
     heavily modified the code. The original code has the following license: */
  
  /* csv - read write comma separated value format
   * Copyright (c) 2003 Michael B. Allen <mba2000 ioplex.com>
   *
   * The MIT License
   * 
   * Permission is hereby granted, free of charge, to any person obtaining a
   * copy of this software and associated documentation files (the "Software"),
   * to deal in the Software without restriction, including without limitation
   * the rights to use, copy, modify, merge, publish, distribute, sublicense,
   * and/or sell copies of the Software, and to permit persons to whom the
   * Software is furnished to do so, subject to the following conditions:
   * 
   * The above copyright notice and this permission notice shall be included
   * in all copies or substantial portions of the Software.
   * 
   * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
   * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
   * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
   * OTHER DEALINGS IN THE SOFTWARE.
   */
  
  private string[]
  parse_row(InputTextline input,	char sep)
  {
    bool quotes = true; /* pay attention to quotes */
    bool inquotes = false;
    bool finished = false, eol = false;
    /* offsets in characters, lengths in bytes */
    uint r = 0, ofs = 0, entry_ofs, entry_len, len;
    ParserState state = ParserState.START;
    
    string[] row = new string[ROW_SIZE_INC];
    
    weak string src = (string)input.utf8_gets();
    if (src == null)
      return null;
    long total_len = src.len(); /* in characters */
    string line = src;
    
    while (!finished) {
      unichar ch = src.get_char();
      uint skip = string.skip[(uchar)ch];
      switch (state) {
        case ParserState.START:
          len = entry_len = entry_ofs = 0;
          if (!eol && ch != sep && ch.isspace()) {
            break; /* skip whitespace */
          } else if (quotes && ch == '"') {
            state = ParserState.COLLECT;
            inquotes = true;
            entry_ofs = ofs + 1;
            break; /* skip initial quote */
          }
          entry_ofs = ofs;
          state = ParserState.COLLECT;
        case ParserState.COLLECT:
          if (inquotes) {
            if (ch == '"') { /* potential end quote encountered */
              state = ParserState.END_QUOTE;
              break;
            }
          } else if (ch == sep || eol) { /* end of entry */
            if (r == row.length)
              row.resize((int)(r + ROW_SIZE_INC));
            // VALABUG: Need to r++ after indexing into array; vala repeats it
            row[r] = line.offset(entry_ofs).ndup(entry_len);
            r++;
            //row.prepend(line.offset(entry_ofs).ndup(entry_len));
            state = ParserState.START;
            inquotes = false;
            if (eol) { /* \n outside of quote, row finished */
              if (r == 1 && entry_len == 0)
                r = 0; /* a single empty entry must be quoted */
              finished = true;
            }
            break;
          } else if (quotes && ch == '"') {
            /*PMNF(errno = EILSEQ, ": unexpected quote in element %d", (r + 1));*/
            critical("unexpected quote in element %d", (r + 1));
            return null;
          }
          if (eol) { /* \n inside quotes, embedded newline, need to read more */
            src = (string)input.utf8_gets();
            if (src == null) {
              critical("expected more lines in element: %d", (r + 1));
              return null;
            }
            line = line.concat("\n", src);
            total_len += src.len() + 1;
          }
          len += skip;
          if (inquotes || !ch.isspace()) { /* ignore trailing whitespace */
            entry_len = len; /* store data */
          }
          break;
        case ParserState.TAILSPACE:
        case ParserState.END_QUOTE:
          if (ch == sep || eol) { /* entry finished */
            if (r == row.length)
              row.resize((int)(r + ROW_SIZE_INC));
            row[r] = line.offset(entry_ofs).ndup(entry_len);
            r++;
            //row.prepend(line.offset(entry_ofs).ndup(entry_len));
            state = ParserState.START;
            inquotes = false;
            if (eol) { /* \n outside of quote, row finished */
              finished = true;
            }
            break;
          } else if (quotes && ch == '"' && state != ParserState.TAILSPACE) {
            entry_len++; /* nope, just an escaped quote */
            state = ParserState.COLLECT;
            break;
          } else if (ch.isspace()) { /* space after end quote */
            state = ParserState.TAILSPACE;
            break;
          }
          /*errno = EILSEQ;
          PMNF(errno, ": bad end quote in element %d", (r + 1));*/
          critical("garbage after end quote in element %d", (r + 1));
          return null;
      }
      if (!eol) /* don't skip first character of new line */
        src = src.next_char();
      ofs++;
      eol = ofs == total_len; // VALABUG: can't use '\0' in comparisons
    }
    
    if (inquotes && state != ParserState.END_QUOTE) {
      critical("missing end quote in element %d", (r + 1));
      return null;
    }
    
    //row.reverse();
    return row;
  }
}