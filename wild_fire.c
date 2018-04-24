# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <math.h>
# include <time.h>
# include <sys/time.h>
# include <unistd.h>

# define NX 300
# define NY 150
#define ANSI_COLOR_RED     "\x1b[41m"
#define ANSI_COLOR_GREEN   "\x1b[42m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_LTGRAY   "\x1b[100m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define CLEAR_SCREEN	   "\e[1;1H\e[2J"

typedef struct {
	char STATE;
	double B,I,D;
	int i,j;
} forest;

void timestamp ( void );
void showforest (int, int, forest[NX][NY]);
void showforest_persist (int, int, forest[NX][NY]);
void p(const char *str);

int main ( void )
{
	int i;
	int j;
	int nx = NX;
	int ny = NY;
	int NSTEPS = 100;
	struct timeval start_time, stop_time, elapsed_time;

	forest t[NX][NY];  
	forest tnew[NX][NY];  

	srand(time(NULL));   
	double Dinit=1;

	gettimeofday(&start_time,NULL);

	/* Initialize forest -- no fire yet */
	for ( j = 0; j < ny; j++ )
	{
		for ( i = 0; i < nx; i++ )
		{
			// Cell sites are aware of their own location. 
			tnew[i][j].i = i;
			tnew[i][j].j = j;

			// No trees around edge of forest
			if( i == 0 || j == 0 || i == nx -1 || j == ny-1 )
			{
				tnew[i][j].STATE = ' ';
			} else {
				// populate forest with trees
				t[i][j].D = tnew[i][j].D = (double) rand() /RAND_MAX;
				t[i][j].B = tnew[i][j].B = 0.5;// (double) rand() /RAND_MAX; 
				t[i][j].I = tnew[i][j].I = 0.95;//(double) rand() /RAND_MAX; 
				if ( Dinit - tnew[i][j].D > 0 ) {
					tnew[i][j].STATE = '^'; 
				} else {
					tnew[i][j].STATE = ' '; 
				}
			}
		}
	}

	/* Start a fire in the middle of the grid */

	/* Constant source fire */ //	tnew[nx/2][ny/2].B = 1;
	tnew[nx/2][ny/2].STATE = 'F';


	showforest(nx,ny,tnew);

	/* Let it burn */
	int it;

	for (it=0 ;it<NSTEPS ;it++ )
	{

		/*
		 *   Save the current burn state.
		 *   */

/*		for ( j = 0; j < ny; j++ )
		{
			for ( i = 0; i < nx; i++ )
			{
				t[i][j].STATE = tnew[i][j].STATE;
			}
		}
*/
		//		memcpy(&t,&tnew,sizeof(tnew));
				memcpy(t,tnew,sizeof(tnew));
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
					// Either a corner neighbor possibly ignites cell
					// or side neighbor;
					// Corner neighbor influence is suppressed but 1/sqrt(2)
					// 0.293 is approx = 1-1/sqrt(2)
					if ( 1*0.293 > (double) rand()/ RAND_MAX) 
					{
						if (t[i-1][j-1].STATE == 'F' ||
								t[i-1][j+1].STATE == 'F' ||
								t[i+1][j-1].STATE == 'F' ||
								t[i+1][j+1].STATE == 'F' )
						{
							if (t[i][j].I > (double) rand() /RAND_MAX)
							{
								tnew[i][j].STATE = 'F';
							}
						}
					} else {
						if (t[i-1][j].STATE == 'F' ||
								t[i][j-1].STATE == 'F' ||
								t[i][j+1].STATE == 'F' ||
								t[i+1][j].STATE == 'F' )
						{
							if (t[i][j].I > (double) rand() /RAND_MAX)
							{
								tnew[i][j].STATE = 'F';
							}
						}
					}

				}

			}

		}
		showforest(nx,ny,tnew);
	}
	gettimeofday(&stop_time,NULL);

	puts("AFTER FIRE");
	showforest_persist(nx,ny,tnew);
	printf ( "\n" );
	printf ( "Almeida et al.,\n \tJournal of Physics: \n\tConference Series 285 (2011) 012038 \n\tdoi:10.1088/1742-6596/285/1/012038:\n" );

	printf ( "\n" );
	timestamp ( );
	timersub(&stop_time, &start_time, &elapsed_time);
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

/******************************************************************************/
void showforest(int nx, int ny, forest tnew[nx][ny])
{
	int i,j;
	for ( j = 0; j < ny; j++ )
	{
		for ( i = 0; i < nx; i++ )
		{
			if (tnew[i][j].STATE == 'F')
			{
				printf(ANSI_COLOR_RED"%c "ANSI_COLOR_RESET,tnew[i][j].STATE);
			}  
			else if (tnew[i][j].STATE == '^')
			{
				printf(ANSI_COLOR_GREEN"%c "ANSI_COLOR_RESET,tnew[i][j].STATE);
			}else if (tnew[i][j].STATE == '.')
			{
				printf(ANSI_COLOR_LTGRAY"%c "ANSI_COLOR_RESET,tnew[i][j].STATE);
			} else {
				printf("%c ",tnew[i][j].STATE);
			}


		}
		printf("\n");
	}

	sleep(1);
	printf(CLEAR_SCREEN);

}
/******************************************************************************/
void showforest_persist(int nx, int ny, forest tnew[nx][ny])
{
	int i,j;
	for ( j = 0; j < ny; j++ )
	{
		for ( i = 0; i < nx; i++ )
		{
			if (tnew[i][j].STATE == 'F')
			{
				printf(ANSI_COLOR_RED"%c "ANSI_COLOR_RESET,tnew[i][j].STATE);
			}
			else if (tnew[i][j].STATE == '^')
			{
				printf(ANSI_COLOR_GREEN"%c "ANSI_COLOR_RESET,tnew[i][j].STATE);
			}else if (tnew[i][j].STATE == '.')
			{
				printf(ANSI_COLOR_LTGRAY"%c "ANSI_COLOR_RESET,tnew[i][j].STATE);
			} else {
				printf("%c ",tnew[i][j].STATE);
			}


		}
		printf("\n");
	}

}

/******************************************************************************/

