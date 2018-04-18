# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <math.h>
# include <time.h>
# include <sys/time.h>

# define NX 50
# define NY 20

void timestamp ( void );

int main ( void )
{
	int i;
	int j;
	int nx = NX;
	int ny = NY;
	int NSTEPS = 1000000;
	struct timeval start_time, stop_time, elapsed_time;

	typedef struct {
		char STATE;
		double B,I,D;
	} tree;

	tree t[NX][NY];  
	tree tnew[NX][NY];  

	srand(time(NULL));   // should only be called once
	double Dinit=0.85;

	gettimeofday(&start_time,NULL);

	/* Initialize forest -- no fire yet */
	for ( j = 0; j < ny; j++ )
	{
		for ( i = 0; i < nx; i++ )
		{
			if( i == 0 || j == 0 || i == nx -1 || j == ny-1 )
			{
                                        tnew[i][j].STATE = ' ';
			} else {
				t[i][j].D = tnew[i][j].D = (double) rand() /RAND_MAX;
				t[i][j].B = tnew[i][j].B = (double) rand() /RAND_MAX; 
				t[i][j].I = tnew[i][j].I = (double) rand() /RAND_MAX; 
				if ( Dinit - tnew[i][j].D > 0 ) {
					tnew[i][j].STATE = '^'; 
				} else {
					tnew[i][j].STATE = ' '; 
				}
			}
		}
	}

	/* Start a fire in the middle of the grid */

	/* Everlasting fire */ //	tnew[nx/2][ny/2].B = 1;
	tnew[nx/2][ny/2].STATE = 'F';

	for ( j = 0; j < ny; j++ )
	{
		for ( i = 0; i < nx; i++ )
		{
			printf("%c ",tnew[i][j].STATE);
		}
		printf("\n");
	}

	/* Let it burn */
	int it;

	for (it=0 ;it<NSTEPS ;it++ )
	{

		/*
		 *   Save the current burn state.
		 *   */

		for ( j = 0; j < ny; j++ )
		{
			for ( i = 0; i < nx; i++ )
			{
				t[i][j].STATE = tnew[i][j].STATE;
			}
		}

		//		memcpy(&t,&tnew,sizeof(tnew));
		//		memcpy(t,tnew,sizeof(tnew));
		/*
		 *  Scan for burning neighbors 
		 *   
		 */
		for ( j = 1; j < ny-1; j++ )
		{
			for ( i = 1; i < nx-1; i++ )
			{
				//If a cell is burning see if it continues burning
				//otherwise the fire goes out. 
				if ( t[i][j].STATE == 'F' )
				{
					if (t[i][j].B < (double) rand() /RAND_MAX)
					{
						tnew[i][j].STATE = '.';
					}	
				}

				// If cell is unburnt but has burning neighbors see
				// if cell ignites.
				if ( t[i][j].STATE == '^' )
				{
					if (t[i-1][j-1].STATE == 'F' ||
							t[i-1][j].STATE == 'F' ||
							t[i-1][j+1].STATE == 'F' ||
							t[i][j-1].STATE == 'F' ||
							t[i][j+1].STATE == 'F' ||
							t[i+1][j-1].STATE == 'F' ||
							t[i+1][j].STATE == 'F' ||
							t[i+1][j+1].STATE == 'F' )
					{
						if (t[i][j].I > (double) rand() /RAND_MAX)
						{
							tnew[i][j].STATE = 'F';
						}
					}
				}

			}

		}
		/*
		   Check for convergence.
		 */

		/*		puts ("************************************************************");
				for ( j = 0; j < ny; j++ )
				{
				for ( i = 0; i < nx; i++ )
				{
				printf("%c ",tnew[i][j].STATE);
				}
				printf("\n");
				}
		 */
	}
	gettimeofday(&stop_time,NULL);

	puts("AFTER FIRE");
	for ( j = 0; j < ny; j++ )
	{
		for ( i = 0; i < nx; i++ )
		{
			printf("%c ",tnew[i][j].STATE);
		}
		printf("\n");
	}

	printf ( "\n" );
	printf ( "Almeida et al.,\n \tJournal of Physics: \n\tConference Series 285 (2011) 012038 \n\tdoi:10.1088/1742-6596/285/1/012038:\n" );

	printf ( "\n" );
	timestamp ( );
	timersub(&stop_time, &start_time, &elapsed_time);
	printf("Elapsed Time: %f \n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);
	return 0;
}
/******************************************************************************/

void timestamp ( void )
{
# define TIME_SIZE 40

	static char time_buffer[TIME_SIZE];
	const struct tm *tm;
	time_t now;

	now = time ( NULL );
	tm = localtime ( &now );

	strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

	printf ( "%s\n", time_buffer );

	return;
# undef TIME_SIZE
}
