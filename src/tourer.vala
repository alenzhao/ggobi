/*

The tour can be broken down into the following components:

 * basic linear algebra (tour.c -> tour-matrix.vala, tour-vector.vala)
 * basis generation (tour.c, tour*pp.c -> tour-basis-generator.vala)
 * interpolation (tour.c -> tour-interpolator.vala)
 * tour itself (= basis generator + interpolator + state, tour.vala)
 * timed tour events (tour-ticker.vala)
 * gui (tour*ui.c -> gui-tour.vala)
 * tour + data (tour*.c -> stage-tour.vala)

= Tour class =

The tour class encapsulates:

 * the basis generation method
 * the interpolation method
 * state, i.e. n previous bases
 * stage tour is connected to
 * variable state for tour (in, out, frozen)
 * methods for manual control

= Tour ticker =

The tour ticker generates a new projection matrix every x seconds, sending a
new-projection signal to all interested parties.

= Tour stage =

The tour stage is a thin wrapper around a TourTicker that projects and caches
the data. The projected columns should be exposed as columns in the data frame
for maximum conveniece. It will also backtransform moved points as necessary.

It should probably also check that the matrices are the correct size.

*/
using GLib;


public class GGobi.Tour : Object {
  public Stage stage;
  public TourState[] states;
  
  public TourBasis basis_generator;
  public TourInterpolation interpolator;
  
  // List of previous target bases
  public SList<TourMatrix> previous_targets;

  public TourMatrix next() {
    if (interpolator.is_finished()) {
      TourMatrix target = basis_generator.generate(states);
      interpolator.set_target(target);
      previous_targets.append(target);
    }
    
    return interpolator.get_next();
  }
  
  // Manual controls
  public void set_position(uint j, double x, double y) {
    

    // previous_targets.append(current);
  }
  
  
  
}