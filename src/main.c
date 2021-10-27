// IMPORTS
#include <vte/vte.h>

// MACROS
#define CLR_R(x)   (((x) & 0xff0000) >> 16)
#define CLR_G(x)   (((x) & 0x00ff00) >>  8)
#define CLR_B(x)   (((x) & 0x0000ff) >>  0)
#define CLR_16(x)  ((double)(x) / 0xff)
#define CLR_GDKA(x, a) (const GdkRGBA){ .red = CLR_16(CLR_R(x)), \
    .green = CLR_16(CLR_G(x)), \
    .blue = CLR_16(CLR_B(x)), \
    .alpha = a }
#define CLR_GDK(x)     CLR_GDKA(x, 0)

// MAGIC
/* I have no clue what this does. */
static void
child_ready(VteTerminal *terminal, GPid pid, GError *error, gpointer user_data)
{
    if (!terminal) return;
    if (pid == -1) gtk_main_quit();
}

// INIT
/* Main function. */
int
main(int argc, char *argv[])
{
    GtkWidget *window, *terminal;

    /* Initialise GTK, the window and the terminal */
    gtk_init(&argc, &argv);
    terminal = vte_terminal_new();
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "scoot-term");

    vte_terminal_set_colors(VTE_TERMINAL(terminal),
        &CLR_GDK(0xeceff4),     // foreground
        &CLR_GDKA(0x2e3440, 1), // background
        (const GdkRGBA[]) {
            /* row 1 */
            CLR_GDK(0x3b4252),  // black
            CLR_GDK(0xbf616a),  // red
            CLR_GDK(0xa3be8c),  // green
            CLR_GDK(0xebcb8b),  // yellow
            CLR_GDK(0x81a1c1),  // blue
            CLR_GDK(0xb48ead),  // purple
            CLR_GDK(0x88c0d0),  // teal
            CLR_GDK(0xeceff4),  // white
            /* row 2 */
            CLR_GDK(0x4c566a),  // black
            CLR_GDK(0xbf616a),  // red
            CLR_GDK(0xa3be8c),  // green
            CLR_GDK(0xebcb8b),  // yellow
            CLR_GDK(0x81a1c1),  // blue
            CLR_GDK(0xb48ead),  // purple
            CLR_GDK(0x8fbcbb),  // teal
            CLR_GDK(0xd8dee9)   // white
        }, 16);

    /* Start a new shell */
    gchar **envp = g_get_environ();
    gchar **command = (gchar *[]){g_strdup(g_environ_getenv(envp, "SHELL")), NULL };
    g_strfreev(envp);
    vte_terminal_spawn_async(VTE_TERMINAL(terminal),
        VTE_PTY_DEFAULT,
        NULL,         /* working directory  */
        command,      /* command */
        NULL,         /* environment */
        0,            /* spawn flags */
        NULL, NULL,   /* child setup */
        NULL,         /* child pid */
        -1,           /* timeout */
        NULL,         /* cancellable */
        child_ready,  /* callback */
        NULL);        /* user_data */

    /* Connect some signals */
    g_signal_connect(window, "delete-event", gtk_main_quit, NULL);
    g_signal_connect(terminal, "child-exited", gtk_main_quit, NULL);

    /* Put widgets together and run the main loop */
    gtk_container_add(GTK_CONTAINER(window), terminal);
    gtk_widget_show_all(window);
    gtk_main();
}
