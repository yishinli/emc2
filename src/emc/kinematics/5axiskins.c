/********************************************************************
* Description: 5axiskins.c
*   Trivial kinematics for 3 axis Cartesian machine
*
*   Derived from a work by Fred Proctor & Will Shackleford
*
* Author:
* License: GPL Version 2
* System: Linux
*    
* Copyright (c) 2007 Chris Radek
*
* Last change:
* $Revision$
* $Author$
* $Date$
********************************************************************/

#include "kinematics.h"		/* these decls */
#include "posemath.h"
#include "rtapi_math.h"

#define d2r(d) ((d)*PM_PI/180.0)
#define r2d(r) ((r)*180.0/PM_PI)

static const double pivot_length = 250.0;

PmCartesian s2r(double r, double t, double p) {
    PmCartesian c;
    t = d2r(t), p = d2r(p);

    c.x = r * sin(p) * cos(t);
    c.y = r * sin(p) * sin(t);
    c.z = r * cos(p);

    return c;
}

int kinematicsForward(const double *joints,
		      EmcPose * pos,
		      const KINEMATICS_FORWARD_FLAGS * fflags,
		      KINEMATICS_INVERSE_FLAGS * iflags)
{
    PmCartesian r = s2r(pivot_length, joints[5] + 90.0, 180.0 - joints[4]);

    pos->tran.x = joints[0] + r.x;
    pos->tran.y = joints[1] + r.y;
    pos->tran.z = joints[2] + r.z + pivot_length;
    pos->a = joints[3];
    pos->b = joints[4];
    pos->c = joints[5];
    pos->u = joints[6];
    pos->v = joints[7];
    pos->w = joints[8];

    return 0;
}

int kinematicsInverse(const EmcPose * pos,
		      double *joints,
		      const KINEMATICS_INVERSE_FLAGS * iflags,
		      KINEMATICS_FORWARD_FLAGS * fflags)
{

    PmCartesian r = s2r(pivot_length, pos->c + 90.0, 180.0 - pos->b);

    joints[0] = pos->tran.x - r.x;
    joints[1] = pos->tran.y - r.y;
    joints[2] = pos->tran.z - r.z - pivot_length;
    joints[3] = pos->a;
    joints[4] = pos->b;
    joints[5] = pos->c;
    joints[6] = pos->u;
    joints[7] = pos->v;
    joints[8] = pos->w;

    return 0;
}

/* implemented for these kinematics as giving joints preference */
int kinematicsHome(EmcPose * world,
		   double *joint,
		   KINEMATICS_FORWARD_FLAGS * fflags,
		   KINEMATICS_INVERSE_FLAGS * iflags)
{
    *fflags = 0;
    *iflags = 0;

    return kinematicsForward(joint, world, fflags, iflags);
}

KINEMATICS_TYPE kinematicsType()
{
    return KINEMATICS_BOTH;
}

#ifdef RTAPI
#include "rtapi.h"		/* RTAPI realtime OS API */
#include "rtapi_app.h"		/* RTAPI realtime module decls */
#include "hal.h"

EXPORT_SYMBOL(kinematicsType);
EXPORT_SYMBOL(kinematicsForward);
EXPORT_SYMBOL(kinematicsInverse);
MODULE_LICENSE("GPL");

int comp_id;
int rtapi_app_main(void) {
    comp_id = hal_init("5axiskins");
    if(comp_id > 0) {
	hal_ready(comp_id);
	return 0;
    }
    return comp_id;
}

void rtapi_app_exit(void) { hal_exit(comp_id); }
#endif