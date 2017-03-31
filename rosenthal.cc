#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

struct input {
    double x_length, y_length, z_length;
    int x_count, y_count, z_count; 
    double eff, V, I, vel, k, a, To;
} rosen_input;

struct domain {
    double x, y, z, T;
};

void get_input(input &user_input);
void initialize_domain(input user_input, domain* domain_init);
void calculate_temperature(input user_input, domain* domain_calc);
void write_file(input user_input, domain* domain_write);

int main()
{
  get_input(rosen_input);
  int node_count = rosen_input.x_count*rosen_input.y_count*rosen_input.z_count;
  domain rosen_domain[node_count];
  initialize_domain(rosen_input, rosen_domain);
  calculate_temperature(rosen_input, rosen_domain);
  write_file(rosen_input, rosen_domain);
  return 0;
}

// Assign mesh, domain, material properties etc.,
void get_input(input &user_input)
{
  user_input.x_length=0.05; user_input.y_length=0.05; user_input.z_length=0.01;
  user_input.x_count=200; user_input.y_count=200; user_input.z_count=2;
  user_input.eff = 1; user_input.V= 60000; user_input.I=0.02; user_input.vel=0.0004;
  user_input.k = 30; user_input.a = 0.00000356; user_input.To=1000;
}

//Initialize the domain by calculating the coordinates and assigning initial temperature of the domain
void initialize_domain(input user_input, domain* domain_init)
{
  double dx = user_input.x_length/user_input.x_count;
  double dy = user_input.y_length/user_input.y_count;
  double dz = user_input.z_length/user_input.z_count;
  int count = 0;
  double temp_x=0, temp_y=0, temp_z=0; 
  for (int i=0; i<user_input.z_count; i++)
    { for (int j=0; j<user_input.y_count; j++)
       { for (int k=0; k<user_input.x_count; k++)
           {
             domain_init[count].x = temp_x;
             domain_init[count].y = temp_y;
             domain_init[count].z = temp_z;
             domain_init[count].T = user_input.To;
             count = ++count;
             temp_x = temp_x + dx;
           } 
        temp_x = 0; temp_y = temp_y + dy;
       }
       temp_y = 0; temp_z = temp_z - dz;
    }
}

//Calculate and update the temperature data of the elements
void calculate_temperature(input user_input, domain* domain_calc)
{
  int count = 0;
  double x_mid = user_input.x_length/2, y_mid = user_input.y_length/2,R;
  for (int i=0; i<user_input.z_count; i++)
    { for (int j=0; j<user_input.y_count; j++)
       { for (int k=0; k<user_input.x_count; k++)
           { R = sqrt(pow((domain_calc[count].x-x_mid),2)+pow((domain_calc[count].y-y_mid),2)+pow((domain_calc[count].z),2));
             domain_calc[count].T = user_input.To + (user_input.eff*user_input.V*user_input.I)/(2*22/7*R*user_input.k)*exp(-user_input.vel/(2*user_input.a)*(R+domain_calc[count].x));
             count = ++count;
           }
       }
    }
}

//Write the output files
void write_file(input user_input, domain* domain_write)
{
  ofstream myfile_3D, myfile_2D;
  myfile_3D.open("output_3D.csv");
  myfile_2D.open("output_2D.csv");
  int count=0;
  for (int i=0; i<user_input.z_count; i++)
    { for (int j=0; j<user_input.y_count; j++)
       { for (int k=0; k<user_input.x_count; k++)
           { myfile_3D<<domain_write[count].x<<","<<domain_write[count].y<<","<<domain_write[count].z<<","<<domain_write[count].T<<endl;
             myfile_2D<<domain_write[count].x<<","<<domain_write[count].y<<","<<domain_write[count].T<<endl;            
             count = count + 1;
           }
       }
     myfile_2D.close();
    }
  myfile_3D.close();
}
