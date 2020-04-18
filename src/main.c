#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <time.h> 

 GtkBuilder      *builder;
 GtkWidget       *window,*dialog,*control,*timer_label,*score_label,*control,*easy,*hard,*grid,*entry,*button[9];
 GtkCssProvider  *provider;
 GdkDisplay      *display;
 GdkScreen       *screen;

char C[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
char randhexc() {return C[rand()%16];}
int score,time_count=0,total_time=15,R3;
gboolean timer_handler();
bool paused,start;
char str[50],css_string[120],score_str[20];


 
void css(char* hex_color1,char* hex_color2)
{


    gtk_style_context_add_provider_for_screen(screen,GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_USER);
    sprintf(css_string,"#c1 {background-color:%s; background-image:none} #c2 {background-color:%s; background-image:none}",hex_color1,hex_color2);       
    gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(provider),css_string, -1, NULL);
}

void set_colors()
{ sprintf(score_str, "Score: %d", score);
  gtk_label_set_text(GTK_LABEL(score_label), score_str);
  char R[8],R2[8];
  R3=rand()%9;
  sprintf(R,"#%c%c%c%c%c%c",randhexc(),randhexc(),randhexc(),randhexc(),randhexc(),randhexc());
  sprintf(R2,"#%c%c%c%c%c%c",randhexc(),randhexc(),randhexc(),randhexc(),randhexc(),randhexc());
   for(int i=0;i<9;i++)
  {
  gtk_widget_set_name(button[i], "c1"); 
  }
  gtk_widget_set_name(button[R3],"c2");
  css(R,R2);
}

int main(int argc, char *argv[])
{
    start=FALSE;
    gtk_init(&argc, &argv);
    srand(time(0)); 

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    timer_label = GTK_WIDGET(gtk_builder_get_object(builder, "timer_label"));
    score_label = GTK_WIDGET(gtk_builder_get_object(builder, "score_label"));
    control = GTK_WIDGET(gtk_builder_get_object(builder, "control"));
    
    button[0] = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
    button[1]= GTK_WIDGET(gtk_builder_get_object(builder, "button2"));
    button[2] = GTK_WIDGET(gtk_builder_get_object(builder, "button3"));
    button[3] = GTK_WIDGET(gtk_builder_get_object(builder, "button4"));
    button[4] = GTK_WIDGET(gtk_builder_get_object(builder, "button5"));
    button[5] = GTK_WIDGET(gtk_builder_get_object(builder, "button6"));
    button[6] = GTK_WIDGET(gtk_builder_get_object(builder, "button7"));
    button[7] = GTK_WIDGET(gtk_builder_get_object(builder, "button8"));
    button[8] = GTK_WIDGET(gtk_builder_get_object(builder, "button9"));

    gtk_builder_connect_signals(builder,NULL);

    g_object_unref(builder);
    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    paused=FALSE;
    for(int i=0;i<9;i++)
    gtk_widget_set_sensitive (button[i], FALSE);
    g_timeout_add_seconds(1, (GSourceFunc)timer_handler, NULL);
    
    
    time_count=0;
    score=0;  

    gtk_widget_show(window);
    gtk_main();
    return 0;
}
void on_response (int argc, char *argv[])
{

  gtk_widget_destroy (dialog);
  time_count=0;
  score=0; 
  for(int i=0;i<9;i++)
  gtk_widget_set_sensitive (button[i], FALSE);
  g_timeout_add_seconds(1, (GSourceFunc)timer_handler, NULL); 
  start=FALSE;
  paused=FALSE;
  gtk_button_set_label(GTK_BUTTON(control), "Start");  
  gtk_style_context_remove_provider_for_screen(screen,GTK_STYLE_PROVIDER(provider));
}
void show_dialog ()
{
  GtkWidget *content_area;
  GtkWidget *label;
  dialog = gtk_dialog_new_with_buttons ("Score",
                                        GTK_WINDOW(window),
                                        GTK_DIALOG_MODAL,
                                        "_OK",
                                        GTK_RESPONSE_OK,
                                        NULL);


  content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
  sprintf(score_str,"Score is %d",score);
  label = gtk_label_new (score_str);
  gtk_container_add (GTK_CONTAINER (content_area), label);

  gtk_widget_show_all (dialog);


  g_signal_connect (GTK_DIALOG (dialog),
                    "response",
                    G_CALLBACK (on_response),
                    NULL);

}
gboolean timer_handler()
{   if(paused==FALSE&&start==TRUE)
    time_count++;
    if(total_time-time_count>total_time/5)
    sprintf(str, "%d", total_time-time_count);
    else
    sprintf(str, "<b><span color=\"red\">%d</span></b>",total_time-time_count);
    gtk_label_set_markup(GTK_LABEL(timer_label), str); 
    if(time_count<total_time)
    return TRUE;
    else
    {show_dialog();
    return FALSE;}
}


void on_window_main_destroy()
{
    gtk_main_quit();

}


void on_control_clicked()
{ if(start==TRUE)
  paused=!paused;
  for(int i=0;i<9;i++)
{
  if(paused==TRUE)
  {gtk_button_set_label(GTK_BUTTON(control), "Continue");
  gtk_widget_set_sensitive (button[i], FALSE);}
  else
   {gtk_button_set_label(GTK_BUTTON(control), "Pause");
   gtk_widget_set_sensitive (button[i], TRUE);}
} 
  set_colors();
  start=TRUE;
}


                


void on_button1_clicked()
{ R3==0?score++:score--;
  set_colors();
}
void on_button2_clicked()
{ R3==1?score++:score--;
  set_colors();
}
void on_button3_clicked()
{ R3==2?score++:score--;
  set_colors();
}
void on_button4_clicked()
{ R3==3?score++:score--;
  set_colors();
}
void on_button5_clicked()
{ R3==4?score++:score--;
  set_colors();
}
void on_button6_clicked()
{ R3==5?score++:score--;
  set_colors();
}
void on_button7_clicked()
{ R3==6?score++:score--;
  set_colors();
}
void on_button8_clicked()
{ R3==7?score++:score--;
  set_colors();
}
void on_button9_clicked()
{ R3==8?score++:score--;
  set_colors();
}
