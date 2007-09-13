/* 
= GUI for imputation stage =


*/

using GLib;
using Gtk;

public class GGobi.GuiImpute : Window {
  public StageImpute stage { construct; get; }
  private Varlist varlist;
  
  construct {
    title = "Imputation";
    set_default_size(400, 250);
    set_border_width(2);
    create_widgets();
  }

  RadioButton fixed;
  RadioButton percent;
  RadioButton mean;
  RadioButton median;
  RadioButton random;
  HScale percent_value;
  SpinButton fixed_value;
  
  public GuiJitter(construct StageImpute stage) {}
  
  public void create_widgets() {
    
    // Initialise variable list 
    varlist = new Varlist(stage);
    // varlist.selection_changed += varlist => {
    //   update_imputation_gui();
    // };
    
    update_imputation_col();
    stage.imputation_changed += (stage, j) => { 
      update_imputation_col();
    };
    varlist.add_view_col("Imputation", 1);

    // Imputation selection radio buttons
    fixed = new RadioButton.with_label(null, "Fixed value:");
    fixed.toggled += fixed => {
      fixed_value.sensitive = fixed.active;
      if (fixed.active) {
        ImputationFixed imp = new ImputationFixed();
        imp.fixed_value = fixed_value.get_value();
        update_imputation(imp);
      }
    };
    
    percent = new RadioButton.with_label(null, "Percent:");
    percent.group = fixed.group;
    percent.toggled += percent => {
      percent_value.sensitive = percent.active;
      if (percent.active) {
        ImputationPercent imp = new ImputationPercent();
        imp.percent = percent_value.get_value();
        update_imputation(imp);
      }
    };
    
    mean = new RadioButton.with_label(null, "Mean");
    mean.group = fixed.group;
    mean.toggled += percent => {
      if (mean.active) update_imputation(new ImputationMean());
    };
        
    median = new RadioButton.with_label(null, "Median");
    median.group = fixed.group;
    median.toggled += percent => {
      if (mean.active) update_imputation(new ImputationMedian());
    };

    random = new RadioButton.with_label(null, "Random");
    random.group = fixed.group;
    random.toggled += percent => {
      if (mean.active) update_imputation(new ImputationRandom());
    };
    
    percent_value = new HScale.with_range(-1, 1, 0.01);
    fixed_value = new SpinButton.with_range(double.MIN, double.MAX, 10);
    fixed_value.sensitive = false;
    
    // Layout -----------------------------------------
    HBox sides = new HBox(false, 3);
    
    VBox left = new VBox(true, 5);
    left.pack_start(varlist.get_ui(), true, true, 0);
    sides.pack_start(left, true, true, 2);
    
    VBox right = new VBox(false, 5);    
    
    HBox fixed_box = new HBox(false, 0);
    fixed_box.pack_start(fixed, false, false, 2);
    fixed_box.pack_start(fixed_value, false, false, 2);

    right.pack_start(fixed_box, false, false, 2);
    
    HBox percent_box = new HBox(false, 0);
    percent_box.pack_start(percent, false, false, 2);
    percent_box.pack_start(percent_value, true, true, 2);
    
    right.pack_start(percent_box, false, false, 2);
    right.pack_start(mean, false, false, 2);
    right.pack_start(median, false, false, 2);
    right.pack_start(random, false, false, 2);
    
    sides.pack_start(right, false, false, 2);
    
    add(sides);
    show_all();    
  }
  
  private void update_imputation(Imputation imp) {
    SList<uint> selected = varlist.selected_vars();
    foreach(uint j in selected) {
      stage.set_imputation(j, imp);
    }
  }
  
  private void update_imputation_col() {
    ListStore vars = varlist.vars;
    TreeIter iter;

    vars.get_iter_first(out iter);
    for(uint j = 0; j < stage.n_cols; j++) {
      vars.set(out iter, 1, stage.imputation[j].description());
      vars.iter_next(out iter);
    }
  }
  
  private void update_imputation_gui() {
    SList<uint> selected = varlist.selected_vars();
    Imputation cur = stage.imputation[(int) selected.data];
    
    switch (((GLib.Object) cur).get_type()) {
      case typeof(ImputationPercent): 
        percent.active = true; 
        percent_value.set_value(((ImputationPercent) cur).percent);
        break;
      case typeof(ImputationFixed): 
        fixed.active = true; 
        fixed_value.set_value(((ImputationFixed) cur).fixed_value);        
        break;
      case typeof(ImputationMean): mean.active = true; break;
      case typeof(ImputationMedian): median.active = true; break;
      case typeof(ImputationRandom): random.active = true; break;
      default: break;
    }
  }
  
}