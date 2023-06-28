#include <gtk/gtk.h>
#include <err.h>
#include <string.h>
#include "image_transformation/image/image_processing.h"
#include "detection/processing.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "Neural/neural2/Neural.h"
#include "Neural/neural2/MathTools.h"
#include "Neural/neural2/Persist.h"
#include "Neural/neural2/DataLoad.h"
#include "sudoku_solver/solver.h"
#include "grid_creation/grid_creation.h"



// Making Global Variables
GtkWidget *Sudoku_Solver_Window;
GtkWidget *Sudoku;
GtkBuilder *builder;
GtkWidget *add_image_button;
GtkWidget *Solve_button;
GtkWidget *Full_screen_button_on;
GtkWidget *Full_screen_button_off;
GtkWidget *dialog;
GtkWidget *Logo;
GtkWidget *Output_image;
GtkWidget *Binarize_and_Rotate_button;
gchar* filename;
const char* button_label;
int button_state;


int main(int argc,char *argv[])
{
    gtk_init(&argc,&argv);
    builder = gtk_builder_new_from_file("Sudoku_Solver_Display.glade");
    Sudoku_Solver_Window = GTK_WIDGET(gtk_builder_get_object(builder,"Sudoku_Solver_Window"));
    g_signal_connect(Sudoku_Solver_Window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
    gtk_builder_connect_signals(builder,NULL);
    gtk_widget_add_events(GTK_WIDGET(Sudoku_Solver_Window), GDK_CONFIGURE);
    Sudoku = GTK_WIDGET(gtk_builder_get_object(builder,"Sudoku_Solver_Fixed"));
    add_image_button = GTK_WIDGET(gtk_builder_get_object(builder,"add_image_button"));
    Full_screen_button_off = GTK_WIDGET(gtk_builder_get_object(builder,"Full_screen_button_off"));
    Full_screen_button_on = GTK_WIDGET(gtk_builder_get_object(builder,"Full_screen_button_on"));
    Solve_button = GTK_WIDGET(gtk_builder_get_object(builder,"Solve"));
    Logo = GTK_WIDGET(gtk_builder_get_object(builder,"Logo"));
    Binarize_and_Rotate_button = GTK_WIDGET(gtk_builder_get_object(builder,"Binarize_and_Rotate_button"));
    g_object_unref(builder);
    gtk_window_set_default_size(GTK_WINDOW(Sudoku_Solver_Window),800,800);
    gtk_window_set_keep_above(GTK_WINDOW(Sudoku_Solver_Window),TRUE);
    gtk_widget_show(Sudoku_Solver_Window);
    gtk_main();

    return EXIT_SUCCESS;
}

void on_Binarize_and_Rotate_button_clicked(GtkButton* button)
{   

    if(Output_image)
    {
      gtk_container_remove(GTK_CONTAINER(Sudoku),Output_image);
    }

    button_label= gtk_button_get_label(button);

    if(button_state == 0)
    {

        SDL_Surface* surface = binarize_and_rotate_gtk(filename);
        SDL_Surface* new = scale(surface);
        IMG_SavePNG(new, "binarized_and_rotated.png");
        SDL_FreeSurface(surface);
        SDL_FreeSurface(new);
        SDL_Quit();
        gtk_widget_hide(Logo);
        Output_image = gtk_image_new_from_file("binarized_and_rotated.png");
        gtk_container_set_border_width(GTK_CONTAINER(Sudoku),10);
        gtk_widget_show(GTK_WIDGET(Output_image));
        gtk_container_add(GTK_CONTAINER(Sudoku),GTK_WIDGET(Output_image));
        gtk_fixed_move(GTK_FIXED(Sudoku),Output_image,200,170);
        gtk_button_set_label(button,"Sobel");
        button_state = 1;
        
    }

    else if(button_state == 1)
    {
        SDL_Surface* from_src = load_image("binarized_and_rotated.png");
        SDL_Surface* surface = sobel(from_src);
        SDL_Surface* new = scale(surface);
        IMG_SavePNG(new, "sobel.png");
        SDL_FreeSurface(surface);
        SDL_FreeSurface(new);
        Output_image = gtk_image_new_from_file("sobel.png");
        gtk_container_set_border_width(GTK_CONTAINER(Sudoku),10);
        gtk_widget_show(GTK_WIDGET(Output_image));
        gtk_container_add(GTK_CONTAINER(Sudoku),GTK_WIDGET(Output_image));
        gtk_fixed_move(GTK_FIXED(Sudoku),Output_image,200,170);
        gtk_button_set_label(button,"Binarize and Rotate");
    }
    
}

void on_Full_screen_button_clicked()
{
    gtk_window_fullscreen(GTK_WINDOW(Sudoku_Solver_Window));
}
void on_Full_screen_button_off_clicked()
{
    gtk_window_unfullscreen(GTK_WINDOW(Sudoku_Solver_Window));
}
void on_add_image_button_clicked(GtkFileChooserButton *f)
{
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f));
    if(Output_image)
    {
      gtk_container_remove(GTK_CONTAINER(Sudoku),Output_image);
    }
    gtk_widget_hide(Logo);
    SDL_Surface* surface = load_image(filename); 
    SDL_Surface* new = scale(surface);
    IMG_SavePNG(new, "binarized_and_rotated.png");
    Output_image = gtk_image_new_from_file("binarized_and_rotated.png");
    gtk_container_set_border_width(GTK_CONTAINER(Sudoku),10);
    gtk_widget_show(GTK_WIDGET(Output_image));
    gtk_container_add(GTK_CONTAINER(Sudoku),GTK_WIDGET(Output_image));
    gtk_fixed_move(GTK_FIXED(Sudoku),Output_image,170,170);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(new);
    SDL_Quit();
}


void on_Solve_clicked()
{
    
    g_print("Solving the Image ");
}
void on_destroy()
{
    gtk_main_quit();
}