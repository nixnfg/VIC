/******************************************************************************
 * @section DESCRIPTION
 *
 * Header file for rvic routing routines
 *
 * @section LICENSE
 *
 * The Variable Infiltration Capacity (VIC) macroscale hydrological model
 * Copyright (C) 2014 The Land Surface Hydrology Group, Department of Civil
 * and Environmental Engineering, University of Washington.
 *
 * The VIC model is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *****************************************************************************/
#ifndef ROUT_RVIC_H
#define ROUT_RVIC_H

#include <vic_def.h>

/******************************************************************************
 * @brief   Routing Structs
 *****************************************************************************/
typedef struct {
    size_t              iSubsetLength;         /*scalar - number of timesteps*/
    size_t              iSources;              /*scalar - number of sources*/
    size_t              iOutlets;              /*scalar - length of subset*/
    int                *source2outlet_ind;    /*1d array - source to outlet mapping*/
    int                *source_y_ind ;        /*1d array - source y location*/
    int                *source_x_ind;         /*1d array - source x location*/
    int                *source_time_offset;   /*1d array - source time offset*/
    double             *unit_hydrograph;   /*2d array[times][sources] - unit hydrographs*/
    double             *aggrunin;          /*2d array[ysize][xsize] - vic runoff flux*/
} rout_param_struct;

/******************************************************************************
 * @brief   main routing Struct
 *****************************************************************************/
typedef struct {
    char                param_filename[MAXSTRING]; 
    rout_param_struct   rout_param; 
    double             *ring;
} rout_struct;

/******************************************************************************
 * @brief   Function prototypes for the rout_rvic extension
 *****************************************************************************/
void rout_start(void);      // read global parameters for routing
void rout_alloc(void);      // allocate memory
void rout_init(void);       // initialize model parameters from parameter files
void rout_run(void);        // run routing over the domain
void rout_write(void);      // write routine for routing
void rout_finalize(void);   // clean up routine for routing

/******************************************************************************
 * @brief   Convolution function adapted from the RVIC scheme
 *****************************************************************************/
void convolve(const size_t nsources,               /*scalar - number of sources*/
                const size_t noutlets,             /*scalar - length of subset*/
                const size_t subset_length,        /*scalar - length of subset*/
                const size_t x_size,
                const int* source2outlet_ind,   /*1d array - source to outlet mapping*/
                const int* source_y_ind,        /*1d array - source y location*/
                const int* source_x_ind,        /*1d array - source x location*/
                const int* source_time_offset,  /*1d array - source time offset*/
                const double* unit_hydrograph,  /*2d array[times][sources] - unit hydrographs*/
                const double* aggrunin,         /*2d array[ysize][xsize] - vic runoff flux*/
                double* ring);
void get_global_param_rout(FILE *gp);
void cshift(double*, int, int);

#endif
