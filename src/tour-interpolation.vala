/* 

= Tour interpolation =

Interpolates between two frames, using geodesic path, as outlined in
http://www-stat.wharton.upenn.edu/~buja/PAPERS/paper-dyn-proj-algs.pdf and
http://www-stat.wharton.upenn.edu/~buja/PAPERS/paper-dyn-proj-math.pdf

We follow the notation outlined in this paper.

  * p = dimension of data
  * d = target dimension
  * F = frame, an orthonormal p x d matrix
  * Fa = starting frame, Fz = target frame
  * ds = dim(span(Fa, Fz)), dI = dim(span(Fa) n span(Fz))
  * Fa'Fz = Va lamda  Vz' (svd)
  * Ga = Fa Va, Gz = Fz Vz
  * tau = principle angles

Currently only works for d = {1, 2}.


*/

class GGobi.TourInterpolation : Object {
  // Frames
  private TourMatrix Fa {get; construct;}
  private TourMatrix Fz {get; construct;}
  // Planes
  private TourMatrix Ga;
  private TourMatrix Gz;
  
  private double current_angle; // in radians
  private double dist; // in radians?
  
  public double delta = Math.PI / 5;  // in radians
  public double[] tau;  // between 0 and 1

  public uint d {get; construct;}
  public uint p {get; construct;}
  
  public TourInterpolation(construct TourMatrix Fa, construct TourMatrix Fz) { }
  
  construct {
    d = Fa.n_cols;
    p = Fa.n_rows;
    tau = new double[d];
    
    pre_project();
  }
  
  private void pre_project() {
    // if (!Fa.is_orthonormal()) return();
    // if (!Fz.is_orthonormal()) return();
    // if (Fa.equivalent(Fz)) return();

    // Compute the SVD: Fa'Fz = Va lambda Vz' --------------------------------

    TourMatrix FatFz = Matrix.multiply_utv(Fa, Fz);
    TourMatrix Va, Vz;
    FatFz.svd(out Va, out lambda, out Vz);

    /* Check span of <Fa,Fz>. If dimension of the intersection is equal to
    dimension of proj, dI=ndim and we should stop here, setting Ft to Fa;
    but this never seems to happen. See page 16 of paper.. */
    
    uint dI = 0; 
    for (i = 0; i < d; i++) { 
     if (lambda[i] > 1.0 - EPSILON) {
       dI++; 
       lambda[i] = 1.0; 
      } 
    }
    
    // Compute frames of principle directions --------------------------------

    Ga = Matrix.multiply_uv(Fa, Va);
    Gz = Matrix.multiply_uv(Fz, Vz);

    // Form an orthogonal coordinate transformation --------------------------

    Ga.orthogonalise();
    // Gz.orthogonalise();

    Gz.orthogonalise_by(Ga);
    Gz.normalise();
    Gz.orthogonalise();

    // Compute, standardize and round principal angles -----------------------
    for (uint i = 0; i < d; i++) {
      tau[i] = acos(lambda);
      if (tau[i] < EPSILON) tau[i] = 0; 
    }

    dist = TourVector.norm(tau);
    // if (dist_az < 0.0001) return(3);
    // Work out relative speeds for each direction
    TourVector.normalise(out tau);
  }
    
  public TourMatrix get_frame(double angle) {
    TourMatrix G = new TourMatrix(d, p);
    for (uint i = 0; i < d; i++) {
      for (uint j = 0; j < p; j++) {
        double value = 
          Ga.get(i, j) * cos(angle * tau[i]) + 
          Gz.get(i, j) * sin(angle * tau[i]);
        G.set(i, j, value);
      }
    }

    // rotate plane to match frame Fa
    TourMatrix F = Matrix.multiply_uv(G, Va);
    // correct round-off errors
    F.normalise(); 
    F.orthogonalise();
    
    return F;
  };


  public void set_target(TourMatrix target) {
    Fz = Fa;
    Fz = target;
    reset();
    pre_project();
  }
  
  public double get_dist() {
    return dist;
  }
  
  public TourMatrix get_next() {
    TourMatrix result = get_interp(current_angle);
    // FIXME: should always finish with target
    if (!is_finished()) current_angle += delta;
    
    return result;
  };

  public bool is_finished() {
    return (current_angle + delta > dist);
  };
  
  public void reset() {
    current_angle = 0;
  };

  // Non-linear mapping.  Specify between 0 and 1.
  public void set_speed(double percent) {
    if (percent < 0.05) {
      step = 0;
    } else if (percent < 0.3) {
      step = (percent - 0.05) / 20;
    } else if (percent < 0.9) { 
      step = pow(percent - 0.3, 1.5) + 0.0125;
    } else {
      step = pow(percent - 0.9, 2.0) + 0.477;
    }

    delta = step * Math.PI / 2;
  }
  
}