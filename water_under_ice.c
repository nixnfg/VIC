/*
 * SUMMARY:      water_under_ice.c 
 * USAGE:        
 *
 * AUTHOR:       Laura Bowling
 * ORG:          University of Washington, Department of Civil Engineering
 * E-MAIL:       nijssen@u.washington.edu
 * ORIG-DATE:     8-Oct-1996 at 08:50:06
 * LAST-MOD: Thu Mar  8 21:35:54 2001 by Keith Cherkauer <cherkaue@u.washington.edu>
 * DESCRIPTION:  
 * DESCRIP-END.
 * FUNCTIONS:   
 * COMMENTS:     
 */

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <vicNl.h>

#if LAKE_MODEL

#if CLOSE_ENERGY
#define MAX_ITER 50
#else
#define MAX_ITER 1
#endif // CLOSE_ENERGY

//static char vcid[] = "$Id$";

/*****************************************************************************
  Function name: water_under_ice()

  Purpose      : 

  Required     :


  Comments     :
*****************************************************************************/
void water_under_ice(int freezeflag, 
		     double sw_ice,
		     double wind,
		     double *Ti,
		     double *water_density,
		     double lat,
		     int numnod,
		     double dz,
		     double Tcutoff,
		     double *qw,
		     double *surface,
		     double *deltaH,
		     double *water_cp,
		     int mixdepth,
		     double hice,
		     double sdepth,
		     double dt)
	      
{
  double Tnew[MAX_LAKE_NODES];
  int k;
  int iterations;
  double jouleold;
  double joulenew;
  double error;
  double energy_out_bottom;
  double de[MAX_LAKE_NODES]; 
  double epsilon = 0.0001;
  double qw_init, qw_mean, qw_final;
  double sw_underice_visible, sw_underice_nir;
  int mixmax;
  
  iterations = 0;

  for(k=0; k<numnod; k++)
    Tnew[k] = Ti[k];

  eddy(freezeflag, wind, Ti, water_density, de, lat, numnod, dz);
  
  qw_init =  0.57*(Ti[0]-Tcutoff)/(SURF/2.);
  *qw = qw_init;
  qw_mean = -999.;

  energycalc(Ti, &jouleold, numnod,dz, surface, water_cp, water_density);
 
  while((fabs(qw_mean - *qw) > epsilon) && iterations < MAX_ITER) {
    
    if(iterations == 0)
      *qw = qw_init;
    else
      *qw = qw_mean;
	
    sw_underice_visible = a1*sw_ice*exp(-1.*(lamisw*hice+lamssw*sdepth));
    sw_underice_nir = a2*sw_ice*exp(-1.*(lamilw*hice+lamslw*sdepth));
	
    /* --------------------------------------------------------------------
     * Calculate the lake temperatures at different levels for the
     * new timestep.
     * -------------------------------------------------------------------- */
	
    temp_area (sw_underice_visible, sw_underice_nir, -1*(*qw) , Ti, Tnew,
	       water_density, de, dt, surface, numnod, 
	       dz, &joulenew, water_cp, &energy_out_bottom);
    *deltaH = (jouleold - joulenew)/(surface[0]*dt*SECPHOUR);
      
    /* --------------------------------------------------------------------
     * Do the convective mixing of the lake water.
     * -------------------------------------------------------------------- */

    tracer_mixer (Tnew, &mixdepth, freezeflag,
		  surface,numnod, dz, water_cp);
    
    if(mixdepth > mixmax) 
      mixmax=mixdepth;

    qw_final = 0.57*(Tnew[0]-Tcutoff)/(SURF/2.);     

    qw_mean = (qw_final + qw_init)/2.;  
    
    iterations += 1;
  }
    
  *qw = qw_mean;
  for(k=0; k<numnod; k++)
    Ti[k] = Tnew[k];
}

#endif // LAKE_MODEL