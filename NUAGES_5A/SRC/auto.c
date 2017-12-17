#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <dirent.h>
#include "compute.h"


int check_ext(char* name)
{
    char* point;
    if((point = strrchr(name, '.')) != NULL )
        return strcmp(point, ".bmp") == 0;

    return 0;
}

void process_img(char* name)
{
    if (!check_ext(name))
        return;

    GError* error = NULL;
    GdkPixbuf* pix = gdk_pixbuf_new_from_file(name, &error);
    if (pix == NULL)
    {
        g_printerr("Error loading file: #%d %s\n", error->code, error->message);
        g_error_free(error);
        exit(1);
    }

    GdkPixbuf* pix_copy = gdk_pixbuf_copy(pix);

    guchar* img_src = gdk_pixbuf_get_pixels(pix);
    guchar* img_dst = gdk_pixbuf_get_pixels(pix_copy);

    int nb_col = gdk_pixbuf_get_width(pix);
    int nb_line = gdk_pixbuf_get_height(pix);

    ImageFormat fmt =
    {
        .nb_col = nb_col,
        .nb_line = nb_line,
        .nb_chan = 3
    };
    double percent = get_percent_clouds(img_src, img_dst, fmt);
    printf("Image: %s\tPercentage of clouds:\t%.2f %%\n", name, percent);
}

int main(void)
{
  DIR *d = opendir(".");
  struct dirent *dir;
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
      process_img(dir->d_name);

    closedir(d);
  }

  return(0);
}