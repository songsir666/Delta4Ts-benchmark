/*
	tot_info -- combine information statistics for multiple tables

	last edit:	89/02/06	D A Gwyn

	SCCS ID:	@(#)tot_info.c	1.1 (edited for publication)
*/
#include "FaultSeeds.h"
#include	<ctype.h>
#include	<stdio.h>

#include	"std.h"

#include	"chisq.h"
#include	"gamma.h"		/* for QChiSq() */

#ifndef FAULT_V6
  #ifndef FAULT_V19
#define	MAXLINE	256
  #else
#define	MAXLINE	26
  #endif
#else
#define	MAXLINE	56 
#endif

#ifndef MAXTBL
#ifndef FAULT_V21
#define	MAXTBL	1000
#else
#define	MAXTBL	5000  
#endif
#endif

static char	line[MAXLINE];		/* row/column header input line */
static long	f[MAXTBL];		/* frequency tallies */
static int	r;			/* # of rows */
static int	c;			/* # of columns */

#define	x(i,j)	f[(i)*c+(j)]		/* convenient way to access freqs */

#define	COMMENT	'#'			/* comment character */


#ifndef NULL
#define NULL 0
#endif

/*ARGSUSED*/
int
main( argc, argv )
	int		argc;
	char		*argv[];
	{
	char	*p;		/* input line scan location */
	int	i;		/* row index */
	int	j;		/* column index */
	double		info;		/* computed information measure */
	int		infodf;		/* degrees of freedom for information */
	double		totinfo = 0.0;	/* accumulated information */
	int		totdf;	/* accumulated degrees of freedom */
 
        totdf = 0;

	while ( fgets( line, MAXLINE, stdin ) != NULL )	/* start new table */
		{
		for ( p = line; *p != '\0' && isspace( (int)*p ); ++p )
			;

		if ( *p == '\0' )
			continue;	/* skip blank line */

		if ( *p == COMMENT )
			{		/* copy comment through */
			(void)fputs( line, stdout );
			continue;
			}

		if ( sscanf( p, "%d %d\n", &r, &c ) != 2 )
			{
			(void)fputs( "* invalid row/column line *\n", stdout );
			return EXIT_FAILURE;
			}
#ifndef FAULT_V3
  #ifndef FAULT_V14
		if ( r * c > MAXTBL )
  #else
		if ( r * c >= MAXTBL )
  #endif
#else
		if ( r * c > MAXTBL-10)
#endif
			{
			(void)fputs( "* table too large *\n", stdout );
			return EXIT_FAILURE;
			}

		/* input tallies */

		for ( i = 0; i < r; ++i )
			for ( j = 0; j < c; ++j )
#ifndef FAULT_V2
				if ( scanf( " %ld", &x(i,j) ) != 1 )
#else
				if ( scanf( " %ld", &x(i,j) ) == 0 )
#endif
					{
					(void)fputs( "* EOF in table *\n",
						     stdout
						   );
					return EXIT_FAILURE;
					}

		/* compute statistic */

		info = InfoTbl( r, c, f, &infodf );

		/* print results */
#ifndef FAULT_V16
		if ( info >= 0.0 )
#else
		if ( info >= 0.1 )
#endif
			{
			(void)printf( "2info = %5.2f\tdf = %2d\tq = %7.4f\n",
				      info, infodf,
				      QChiSq( info, infodf )
				    );
#ifndef FAULT_V5
			totinfo += info;
#else
			totinfo = info; /* removed + before = */
#endif

#ifndef FAULT_V9
			totdf += infodf;
#else
			totdf = infodf;
#endif
			}
		else
			(void)fputs( info < -3.5 ? "out of memory\n"
				   : info < -2.5 ? "table too small\n"
				   : info < -1.5 ? "negative freq\n"
				   : "table all zeros\n",
				     stdout
				   );
		}

	if ( totdf <= 0 )
		{
		(void)fputs( "\n*** no information accumulated ***\n", stdout );
		return EXIT_FAILURE;
		}

	(void)printf( "\ntotal 2info = %5.2f\tdf = %2d\tq = %7.4f\n",
		      totinfo, totdf,
		      QChiSq( totinfo, totdf )
		    );
	return EXIT_SUCCESS;
	}


/*  -*- Last-Edit:  Tue Dec 15 14:48:14 1992 by Tarak S. Goradia; -*- */

/*
	Gamma -- gamma and related functions

	last edit:	88/09/09	D A Gwyn

	SCCS ID:	@(#)gamma.c	1.1 (edited for publication)

Acknowledgement:
	Code based on that found in "Numerical Methods in C".
*/

#include	<math.h>
#include        <stdio.h>

#include	"std.h"

double
LGamma( x )
	double			x;
	{
	static const double	cof[6] =
		{
		76.18009173,	-86.50532033,	24.01409822,
		-1.231739516,	0.120858003e-2,	-0.536382e-5
		};
	double			tmp, ser;
	int		j;


	if ( --x < 0.0 )	/* use reflection formula for accuracy */
		{
		double	pix = PI * x;

		return log( pix / sin( pix ) ) - LGamma( 1.0 - x );
		}

	tmp = x + 5.5;
	tmp -= (x + 0.5) * log( tmp );

	ser = 1.0;

	for ( j = 0; j < 6; ++j )
		ser += cof[j] / ++x;
#ifndef FAULT_V12
	return -tmp + log( 2.50662827465 * ser );
#else
	return -tmp + log( 2.50663 * ser );
#endif
	}

#define	ITMAX	100
#define	EPS	3.0e-7

static double
gser( a, x )
	double		a, x;
	{
	double		ap, del, sum;
	int	n;


	if ( x <= 0.0 )
		return 0.0;

	del = sum = 1.0 / (ap = a);

	for ( n = 1; n <= ITMAX; ++n )
		{
#ifndef FAULT_V11
		sum += del *= x / ++ap;
#else
		sum = del *= x / ++ap;
#endif

#ifndef FAULT_V8
  #ifndef FAULT_V15
		if ( Abs( del ) < Abs( sum ) * EPS )
  #else
		if ( Abs( del ) < Abs( sum ) * (EPS-.000001) )
  #endif
			return sum * exp( -x + a * log( x ) - LGamma( a ) );
#else
		if ( Abs(del) < Abs(sum) * EPS )
			return sum * exp(  x + a * log( x ) - LGamma( a ) );
#endif
		}

	/*NOTREACHED*/
	}

static double
gcf( a, x )
	double		a, x;
	{
	int	n;
	double		gold = 0.0, fac = 1.0, b1 = 1.0,
			b0 = 0.0, a0 = 1.0, a1 = x;
#ifndef FAULT_V23
	for ( n = 1; n <= ITMAX; ++n )
#else
	for ( n = 0; n <= ITMAX; ++n )
#endif
		{
		double	anf;
		double	an = (double)n;
		double	ana = an - a;

		a0 = (a1 + a0 * ana) * fac;
		b0 = (b1 + b0 * ana) * fac;
#ifndef FAULT_V17
		anf = an * fac;
#else
		anf = an - fac;
#endif
		b1 = x * b0 + anf * b1;
		a1 = x * a0 + anf * a1;

		if ( a1 != 0.0 )
			{		/* renormalize */
			double	g = b1 * (fac = 1.0 / a1);

			gold = g - gold;
#ifndef FAULT_V4
			if ( Abs( gold ) < EPS * Abs( g ) )
#else
			if ( Abs( gold ) < Abs( g ) )
#endif
				return exp( -x + a * log( x ) - LGamma( a ) ) * g;

			gold = g;
			}
		}

	/*NOTREACHED*/
	}

double
QGamma( a, x )
	double	a, x;
	{

	return x < a + 1.0 ? 1.0 - gser( a, x ) : gcf( a, x );
	}

double
QChiSq( chisq, df )
	double	chisq;
	int	df;
	{
	return QGamma( (double)df / 2.0, chisq / 2.0 );
	}


/*
	InfoTbl -- Kullback's information measure for a 2-way contingency table

	last edit:	88/09/19	D A Gwyn

	SCCS ID:	@(#)info.c	1.1 (edited for publication)

	Special return values:
		-1.0	entire table consisted of 0 entries
		-2.0	invalid table entry (frequency less than 0)
		-3.0	invalid table dimensions (r or c less than 2)
		-4.0	unable to allocate enough working storage
*/

#include	<math.h>		/* for log() */
#if __STDC__
#include	<stdlib.h>		/* malloc, free */

#include	"std.h"
#else
#include	"std.h"

extern pointer	malloc();
extern void	free();
#endif

#ifndef NULL
#define NULL 0
#endif

#define	x(i,j)	f[(i)*c+(j)]		/* convenient way to access freqs */

double
InfoTbl( r, c, f, pdf )
	int		r;		/* # rows in table */
	int		c;		/* # columns in table */
	const long	*f;		/* -> r*c frequency tallies */
	int		*pdf;		/* -> return # d.f. for chi-square */
	{
	int	i;		/* row index */
	int	j;		/* column index */
#ifndef FAULT_V10
	double		N;		/* (double)n */
#else
	float		N;		/* (double)n */
#endif
	double		info;		/* accumulates information measure */
	double		*xi;		/* row sums */
	double		*xj;		/* col sums */
	int		rdf = r - 1;	/* row degrees of freedom */
	int		cdf = c - 1;	/* column degrees of freedom */
#ifndef FAULT_V18
  #ifndef FAULT_V20
	if ( rdf <= 0 || cdf <= 0 )
  #else
	if ( rdf <= 0  )
  #endif
#else
	if ( rdf == 0 || cdf == 0 )
#endif
		{
		info = -3.0;
		goto ret3;
		}

	*pdf = rdf * cdf;		/* total degrees of freedom */

	if ( (xi = (double *)malloc( r * sizeof(double) )) == NULL )
		{
		info = -4.0;
		goto ret3;
		}

	if ( (xj = (double *)malloc( c * sizeof(double) )) == NULL )
		{
		info = -4.0;
		goto ret2;
		}

	/* compute row sums and total */

	N = 0.0;

	for ( i = 0; i < r; ++i )
		{
		double	sum = 0.0;	/* accumulator */

		for ( j = 0; j < c; ++j )
			{
			long	k = x(i,j);

			if ( k < 0L )
				{
				info = -2.0;
#ifndef FAULT_V1
				goto ret1;
#endif
				}

			sum += (double)k;
			}

		N += xi[i] = sum;
		}
#ifndef FAULT_V22
	if ( N <= 0.0 )
#else
	if ( N <= 1.0 )
#endif
		{
		info = -1.0;
		goto ret1;
		}

	/* compute column sums */

	for ( j = 0; j < c; ++j )
		{
		double	sum = 0.0;	/* accumulator */

		for ( i = 0; i < r; ++i )
			sum += (double)x(i,j);

		xj[j] = sum;
		}

	/* compute information measure (four parts) */

	info = N * log( N );					/* part 1 */

	for ( i = 0; i < r; ++i )
		{
		double	pi = xi[i];	/* row sum */
#ifndef FAULT_V7
		if ( pi > 0.0 )
#else
		if ( pi >= 0.0 ) /* added an = after > */
#endif
			info -= pi * log( pi );			/* part 2 */

		for ( j = 0; j < c; ++j )
			{
			double	pij = (double)x(i,j);

			if ( pij > 0.0 )
				info += pij * log( pij );	/* part 3 */
			}
		}

	for ( j = 0; j < c; ++j )
		{
		double	pj = xj[j];	/* column sum */
#ifndef FAULT_V13
		if ( pj > 0.0 )
#else
		if ( pj >= 0.0 )
#endif
			info -= pj * log( pj );			/* part 4 */
		}

	info *= 2.0;			/* for comparability with chi-square */

    ret1:
	free( (pointer)xj );
    ret2:
	free( (pointer)xi );
    ret3:
	return info;
	}
