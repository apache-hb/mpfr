/* Test file for mpfr_atan2u.

Copyright 2021 Free Software Foundation, Inc.
Contributed by the AriC and Caramba projects, INRIA.

This file is part of the GNU MPFR Library.

The GNU MPFR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MPFR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MPFR Library; see the file COPYING.LESSER.  If not, see
https://www.gnu.org/licenses/ or write to the Free Software Foundation, Inc.,
51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA. */

#include "mpfr-test.h"

#if 0
#define TEST_FUNCTION mpfr_atan2u
#define TWO_ARGS
#define ULONG_ARG2
#include "tgeneric.c"
#endif

/* Special cases from IEEE 754-2019 (page 63) */
static void
check_ieee754 (void)
{
  mpfr_t x, y, z;
  int i;
  mpfr_prec_t prec = 2; /* we need at least 2 so that 3/4 is exact */

  mpfr_init2 (x, prec);
  mpfr_init2 (y, prec);
  mpfr_init2 (z, prec);

  /* atan2Pi(±0, −0) is ±1 */
  mpfr_set_zero (x, -1);
  mpfr_set_zero (y, +1);
  mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
  if (mpfr_cmp_ui (z, 1) != 0)
    {
      printf ("Error for atan2u(+0,-0,2)\n");
      printf ("expected 1\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }
  mpfr_set_zero (y, -1);
  mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
  if (mpfr_cmp_si (z, -1) != 0)
    {
      printf ("Error for atan2u(-0,-0,2)\n");
      printf ("expected -1\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }

  /* atan2Pi(±0, +0) is ±0 */
  mpfr_set_zero (x, +1);
  mpfr_set_zero (y, +1);
  mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
  if (!MPFR_IS_ZERO(z) || MPFR_IS_NEG(z))
    {
      printf ("Error for atan2u(+0,+0,2)\n");
      printf ("expected +0\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }
  mpfr_set_zero (y, -1);
  mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
  if (!MPFR_IS_ZERO(z) || MPFR_IS_POS(z))
    {
      printf ("Error for atan2u(-0,+0,2)\n");
      printf ("expected -0\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }

  /* atan2Pi(±0, x) is ±1 for x < 0 (including x=-Inf) */
  for (i = 0; i < 2; i++)
    {
      if (i == 0)
        mpfr_set_si (x, -1, MPFR_RNDN);
      else
        mpfr_set_inf (x, -1);
      mpfr_set_zero (y, +1);
      mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
      if (mpfr_cmp_ui (z, 1) != 0)
        {
          mpfr_printf ("Error for atan2u(+0,%Re,2)\n", x);
          printf ("expected 1\n");
          printf ("got     "); mpfr_dump (z);
          exit (1);
        }
      mpfr_set_zero (y, -1);
      mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
      if (mpfr_cmp_si (z, -1) != 0)
        {
          mpfr_printf ("Error for atan2u(-0,%Re,2)\n", x);
          printf ("expected -1\n");
          printf ("got     "); mpfr_dump (z);
          exit (1);
        }
    }

  /* atan2Pi(±0, x) is ±0 for x > 0 (including x=+Inf) */
  for (i = 0; i < 2; i++)
    {
      if (i == 0)
          mpfr_set_ui (x, 1, MPFR_RNDN);
      else
        mpfr_set_inf (x, 1);
      mpfr_set_zero (y, +1);
      mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
      if (!MPFR_IS_ZERO(z) || MPFR_IS_NEG(z))
        {
          mpfr_printf ("Error for atan2u(+0,%Re,2)\n", x);
          printf ("expected +0\n");
          printf ("got     "); mpfr_dump (z);
          exit (1);
        }
      mpfr_set_zero (y, -1);
      mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
      if (!MPFR_IS_ZERO(z) || MPFR_IS_POS(z))
        {
          mpfr_printf ("Error for atan2u(-0,%Re,2)\n", x);
          printf ("expected -0\n");
          printf ("got     "); mpfr_dump (z);
          exit (1);
        }
    }

  /* atan2Pi(y, ±0) is −½ for y < 0 */
  mpfr_set_si (y, -1, MPFR_RNDN);
  mpfr_set_zero (x, 1);
  mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
  if (mpfr_cmp_si_2exp (z, -1, -1) != 0)
    {
      printf ("Error for atan2u(-1,+0,2)\n");
      printf ("expected -1/2\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }
  mpfr_set_zero (x, -1);
  mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
  if (mpfr_cmp_si_2exp (z, -1, -1) != 0)
    {
      printf ("Error for atan2u(-1,-0,2)\n");
      printf ("expected -1/2\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }

  /* atan2Pi(y, ±0) is +½ for y > 0 */
  mpfr_set_ui (y, 1, MPFR_RNDN);
  mpfr_set_zero (x, 1);
  mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
  if (mpfr_cmp_ui_2exp (z, 1, -1) != 0)
    {
      printf ("Error for atan2u(1,+0,2)\n");
      printf ("expected 1/2\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }
  mpfr_set_zero (x, -1);
  mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
  if (mpfr_cmp_ui_2exp (z, 1, -1) != 0)
    {
      printf ("Error for atan2u(-1,-0,2)\n");
      printf ("expected 1/2\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }

  /* atan2Pi(±y, −∞) is ±1 for finite y > 0 */
  mpfr_set_inf (x, -1);
  mpfr_set_ui (y, 1, MPFR_RNDN);
  mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
  if (mpfr_cmp_ui (z, 1) != 0)
    {
      printf ("Error for atan2u(1,-Inf,2)\n");
      printf ("expected 1\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }
  mpfr_set_si (y, -1, MPFR_RNDN);
  mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
  if (mpfr_cmp_si (z, -1) != 0)
    {
      printf ("Error for atan2u(-1,-Inf,2)\n");
      printf ("expected -1\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }

  /* atan2Pi(±y, +∞) is ±0 for finite y > 0 */
  mpfr_set_inf (x, +1);
  mpfr_set_ui (y, 1, MPFR_RNDN);
  mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
  if (!MPFR_IS_ZERO(z) || MPFR_IS_NEG(z))
    {
      printf ("Error for atan2u(1,+Inf,2)\n");
      printf ("expected +0\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }
  mpfr_set_si (y, -1, MPFR_RNDN);
  mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
  if (!MPFR_IS_ZERO(z) || MPFR_IS_POS(z))
    {
      printf ("Error for atan2u(-1,+Inf,2)\n");
      printf ("expected -0\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }

  /* atan2Pi(±∞, x) is ±½ for finite x */
  for (i = 0; i < 4; i++)
    {
      if (i == 0)
        mpfr_set_ui (x, 1, MPFR_RNDN);
      else if (i == 1)
        mpfr_set_zero (x, 1);
      else if (i == 2)
        mpfr_set_zero (x, -1);
      else
        mpfr_set_si (x, -1, MPFR_RNDN);
      mpfr_set_inf (y, 1);
      mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
      if (mpfr_cmp_ui_2exp (z, 1, -1) != 0)
        {
          mpfr_printf ("Error for atan2u(+Inf,%Re,2)\n", x);
          printf ("expected 1/2\n");
          printf ("got     "); mpfr_dump (z);
          exit (1);
        }
      mpfr_set_inf (y, -1);
      mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
      if (mpfr_cmp_si_2exp (z, -1, -1) != 0)
        {
          mpfr_printf ("Error for atan2u(-Inf,%Re,2)\n", x);
          printf ("expected -1/2\n");
          printf ("got     "); mpfr_dump (z);
          exit (1);
        }
    }

  /* atan2Pi(±∞, −∞) is ±¾ */
  mpfr_set_inf (x, -1);
  mpfr_set_inf (y, 1);
  mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
  if (mpfr_cmp_ui_2exp (z, 3, -2) != 0)
    {
      printf ("Error for atan2u(+Inf,-Inf,2)\n");
      printf ("expected 3/4\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }
  mpfr_set_inf (y, -1);
  mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
  if (mpfr_cmp_si_2exp (z, -3, -2) != 0)
    {
      printf ("Error for atan2u(+Inf,-Inf,2)\n");
      printf ("expected -3/4\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }

  /* atan2Pi(±∞, +∞) is ±¼ */
  mpfr_set_inf (x, 1);
  mpfr_set_inf (y, 1);
  mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
  if (mpfr_cmp_ui_2exp (z, 1, -2) != 0)
    {
      printf ("Error for atan2u(+Inf,+Inf,2)\n");
      printf ("expected 1/4\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }
  mpfr_set_inf (y, -1);
  mpfr_atan2u (z, y, x, 2, MPFR_RNDN);
  if (mpfr_cmp_si_2exp (z, -1, -2) != 0)
    {
      printf ("Error for atan2u(+Inf,+Inf,2)\n");
      printf ("expected -1/4\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }

  mpfr_clear (x);
  mpfr_clear (y);
  mpfr_clear (z);
}

/* Special cases from LIA-2 (5.3.9.15) not covered by IEEE 754-2019.
   Warning: in LIA-2, the order of arguments is reversed:
   arcu (u,x,y) corresponds to atan2u (y,x,u). */
static void
check_lia2 (void)
{
  mpfr_t x, y, z;
  mpfr_prec_t prec = 9; /* 3*99 must be exactly representable */
  unsigned long u;
  int r, inex;

  mpfr_init2 (x, prec);
  mpfr_init2 (y, prec);
  mpfr_init2 (z, prec);

  /* arcu (u, x, x) = u/8 for x > 0 */
  for (u = 1; u < 100; u++)
     RND_LOOP (r)
       {
         mpfr_set_ui (x, 1, MPFR_RNDN); /* exact */
         mpfr_set_ui (y, 1, MPFR_RNDN); /* exact */
         mpfr_atan2u (z, y, x, u, (mpfr_rnd_t) r);
         inex = mpfr_set_ui (x, u, MPFR_RNDN);
         MPFR_ASSERTN(inex == 0);
         inex = mpfr_div_ui (x, x, 8, (mpfr_rnd_t) r);
         if ((r != MPFR_RNDF || inex == 0) && !mpfr_equal_p (x, z))
           {
             printf ("Error: atan2u(1,1,u) != u/8 for u=%lu and rnd=%s\n",
                     u, mpfr_print_rnd_mode ((mpfr_rnd_t) r));
             printf ("got: "); mpfr_dump (z);
             exit (1);
           }
       }

  /* arcu (u, x, -x) = -u/8 for x > 0 */
  for (u = 1; u < 100; u++)
     RND_LOOP (r)
       {
         mpfr_set_ui (x, 1, MPFR_RNDN); /* exact */
         mpfr_set_si (y, -1, MPFR_RNDN); /* exact */
         mpfr_atan2u (z, y, x, u, (mpfr_rnd_t) r);
         inex = mpfr_set_ui (x, u, MPFR_RNDN);
         MPFR_ASSERTN(inex == 0);
         inex = mpfr_div_si (x, x, -8, (mpfr_rnd_t) r);
         if ((r != MPFR_RNDF || inex == 0) && !mpfr_equal_p (x, z))
           {
             printf ("Error: atan2u(-1,1,u) != -u/8 for u=%lu and rnd=%s\n",
                     u, mpfr_print_rnd_mode ((mpfr_rnd_t) r));
             printf ("got: "); mpfr_dump (z);
             exit (1);
           }
       }

  /* arcu (u, x, −x) = 3 · u/8 for x < 0 */
  for (u = 1; u < 100; u++)
     RND_LOOP (r)
       {
         mpfr_set_si (x, -1, MPFR_RNDN); /* exact */
         mpfr_set_ui (y, 1, MPFR_RNDN); /* exact */
         mpfr_atan2u (z, y, x, u, (mpfr_rnd_t) r);
         inex = mpfr_set_ui (x, 3 * u, MPFR_RNDN);
         MPFR_ASSERTN(inex == 0);
         inex = mpfr_div_ui (x, x, 8, (mpfr_rnd_t) r);
         if ((r != MPFR_RNDF || inex == 0) && !mpfr_equal_p (x, z))
           {
             printf ("Error: atan2u(1,-1,u) != 3u/8 for u=%lu and rnd=%s\n",
                     u, mpfr_print_rnd_mode ((mpfr_rnd_t) r));
             printf ("got: "); mpfr_dump (z);
             exit (1);
           }
       }
  
  /* arcu (u, x, x) = -3 · u/8 for x < 0 */
  for (u = 1; u < 100; u++)
     RND_LOOP (r)
       {
         mpfr_set_si (x, -1, MPFR_RNDN); /* exact */
         mpfr_set_si (y, -1, MPFR_RNDN); /* exact */
         mpfr_atan2u (z, y, x, u, (mpfr_rnd_t) r);
         inex = mpfr_set_ui (x, 3 * u, MPFR_RNDN);
         MPFR_ASSERTN(inex == 0);
         inex = mpfr_div_si (x, x, -8, (mpfr_rnd_t) r);
         if ((r != MPFR_RNDF || inex == 0) && !mpfr_equal_p (x, z))
           {
             printf ("Error: atan2u(-1,-1,u) != -3u/8 for u=%lu and rnd=%s\n",
                     u, mpfr_print_rnd_mode ((mpfr_rnd_t) r));
             printf ("got: "); mpfr_dump (z);
             exit (1);
           }
       }
  
  mpfr_clear (x);
  mpfr_clear (y);
  mpfr_clear (z);
}

static void
check_random (void)
{
  mpfr_t x, y, z;
  long i, j, B = 20;
  unsigned long u;
  int r;

  mpfr_init2 (x, 53);
  mpfr_init2 (y, 53);
  mpfr_init2 (z, 53);

  /* hard-coded values */

  /* first quadrant */
  mpfr_set_ui (x, 17, MPFR_RNDN);
  mpfr_set_ui (y, 42, MPFR_RNDN);
  mpfr_atan2u (z, y, x, 360, MPFR_RNDN); /* in degrees */
  mpfr_set_str (x, "4.3f6b9d4ccd754p+4", 16, MPFR_RNDN);
  if (!mpfr_equal_p (z, x))
    {
      printf ("Error for atan2u(42,17,360)\n");
      printf ("expected "); mpfr_dump (x);
      printf ("got      "); mpfr_dump (z);
      exit (1);
    }

  /* second quadrant */
  mpfr_set_si (x, -17, MPFR_RNDN);
  mpfr_set_ui (y, 42, MPFR_RNDN);
  mpfr_atan2u (z, y, x, 360, MPFR_RNDN); /* in degrees */
  mpfr_set_str (x, "7.009462b3328acp+4", 16, MPFR_RNDN);
  if (!mpfr_equal_p (z, x))
    {
      printf ("Error for atan2u(42,-17,360)\n");
      printf ("expected "); mpfr_dump (x);
      printf ("got      "); mpfr_dump (z);
      exit (1);
    }

  /* third quadrant */
  mpfr_set_si (x, -17, MPFR_RNDN);
  mpfr_set_si (y, -42, MPFR_RNDN);
  mpfr_atan2u (z, y, x, 360, MPFR_RNDN); /* in degrees */
  mpfr_set_str (x, "-7.009462b3328acp+4", 16, MPFR_RNDN);
  if (!mpfr_equal_p (z, x))
    {
      printf ("Error for atan2u(-42,-17,360)\n");
      printf ("expected "); mpfr_dump (x);
      printf ("got      "); mpfr_dump (z);
      exit (1);
    }

  /* fourth quadrant */
  mpfr_set_ui (x, 17, MPFR_RNDN);
  mpfr_set_si (y, -42, MPFR_RNDN);
  mpfr_atan2u (z, y, x, 360, MPFR_RNDN); /* in degrees */
  mpfr_set_str (x, "-4.3f6b9d4ccd754p+4", 16, MPFR_RNDN);
  if (!mpfr_equal_p (z, x))
    {
      printf ("Error for atan2u(-42,17,360)\n");
      printf ("expected "); mpfr_dump (x);
      printf ("got      "); mpfr_dump (z);
      exit (1);
    }

  /* case u=0 with x > 0 */
  mpfr_set_ui (x, 17, MPFR_RNDN);
  mpfr_set_ui (y, 42, MPFR_RNDN);
  mpfr_atan2u (z, y, x, 0, MPFR_RNDN);
  if (!MPFR_IS_ZERO(z) || MPFR_IS_NEG(z))
    {
      printf ("Error for atan2u(42,17,0)\n");
      printf ("expected +0\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }
  mpfr_set_si (y, -42, MPFR_RNDN);
  mpfr_atan2u (z, y, x, 0, MPFR_RNDN);
  if (!MPFR_IS_ZERO(z) || MPFR_IS_POS(z))
    {
      printf ("Error for atan2u(-42,17,0)\n");
      printf ("expected -0\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }

  /* case u=0 with x < 0 */
  mpfr_set_si (x, -17, MPFR_RNDN);
  mpfr_set_ui (y, 42, MPFR_RNDN);
  mpfr_atan2u (z, y, x, 0, MPFR_RNDN);
  if (mpfr_cmp_ui (z, 1) != 0)
    {
      printf ("Error for atan2u(42,-17,0)\n");
      printf ("expected +1\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }
  mpfr_set_si (y, -42, MPFR_RNDN);
  mpfr_atan2u (z, y, x, 0, MPFR_RNDN);
  if (mpfr_cmp_si (z, -1) != 0)
    {
      printf ("Error for atan2u(-42,-17,0)\n");
      printf ("expected -1\n");
      printf ("got     "); mpfr_dump (z);
      exit (1);
    }

  for (i = -B; i <= B; i++)
    {
      mpfr_set_si (x, i, MPFR_RNDN);
      for (j = -B; j <= B; j++)
        {
          mpfr_set_si (y, j, MPFR_RNDN);
          for (u = 0; u <= B; u++)
            RND_LOOP (r)
              mpfr_atan2u (z, y, x, u, (mpfr_rnd_t) r);
        }
    }

  mpfr_clear (x);
  mpfr_clear (y);
  mpfr_clear (z);
}

int
main (void)
{
  tests_start_mpfr ();

  check_ieee754 ();

  check_lia2 ();

  check_random ();

  tests_end_mpfr ();
  return 0;
}