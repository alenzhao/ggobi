
/*
 A simple application that reads data in the original format
 of xgobi and converts it to the XML format used by ggobi.
 */


#include <gtk/gtk.h>
#include "write_xml.h"

gint
main (gint argc, gchar *argv[])
{
  ggobid *gg;
  char *fileName;
  XmlWriteInfo info;
  gtk_init (&argc, &argv);
  gg = ggobi_alloc ();

  initSessionOptions();
  sessionOptions->cmdArgs = argv;
  sessionOptions->numArgs = argc;

  parse_command_line(&argc, argv, gg);

  fileName = argv[1];

  gg->displays = NULL;
  globals_init (gg); /*-- variables that don't depend on the data --*/
  color_table_init (gg);

  fileset_read (fileName, unknown_data, gg);

  memset(&info, '\0', sizeof(XmlWriteInfo));
  info.useDefault = true;
  write_xml_stream (stdout, gg, NULL, &info);

  return(0);
}
