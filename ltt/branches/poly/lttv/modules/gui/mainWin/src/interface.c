/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget*
create_MWindow (void)
{
  GtkWidget *MWindow;
  GtkWidget *MVbox;
  GtkWidget *MMenuBox;
  GtkWidget *MenuMain;
  GtkWidget *FileMenuTitle;
  GtkWidget *FileMenuTitle_menu;
  GtkWidget *FileMenuNewTitle;
  GtkWidget *FileMenuNewTitle_menu;
  GtkWidget *EmptyTraceset;
  GtkWidget *CloneTraceset;
  GtkWidget *FileMenuNewSep;
  GtkWidget *Tab;
  GtkWidget *OpenTraceset;
  GtkWidget *Close;
  GtkWidget *CloseTab;
  GtkWidget *FileMenuSeparator1;
  GtkWidget *AddTrace;
  GtkWidget *RemoveTrace;
  GtkWidget *Save;
  GtkWidget *SaveAs;
  GtkWidget *FileMenuSeparator2;
  GtkWidget *Quit;
  GtkWidget *EditMenuTitle;
  GtkWidget *EditMenuTitle_menu;
  GtkWidget *Cut;
  GtkWidget *Copy;
  GtkWidget *Paste;
  GtkWidget *Delete;
  GtkWidget *ViewMenuTitle;
  GtkWidget *ViewMenuTitle_menu;
  GtkWidget *ZoomIn;
  GtkWidget *ZoomOut;
  GtkWidget *ZoomExtended;
  GtkWidget *ViewMenuSeparator;
  GtkWidget *GoToTime;
  GtkWidget *ShowTimeFrame;
  GtkWidget *ToolMenuTitle;
  GtkWidget *ToolMenuTitle_menu;
  GtkWidget *MoveViewerUp;
  GtkWidget *MoveViewerDown;
  GtkWidget *RemoveViewer;
  GtkWidget *ToolMenuSeparator;
  GtkWidget *insert_viewer_test;
  GtkWidget *PluginMenuTitle;
  GtkWidget *PluginMenuTitle_menu;
  GtkWidget *LoadModule;
  GtkWidget *UnloadModule;
  GtkWidget *AddModuleSearchPath;
  GtkWidget *OptionMenuTitle;
  GtkWidget *OptionMenuTitle_menu;
  GtkWidget *Color;
  GtkWidget *OptMenuSeparator;
  GtkWidget *OpenFilter;
  GtkWidget *SaveConfiguration;
  GtkWidget *MenuHelp;
  GtkWidget *HelpMenuTitle;
  GtkWidget *HelpMenu;
  GtkWidget *Content;
  GtkWidget *HelpmenuSeparator;
  GtkWidget *About;
  GtkWidget *MToolbar1;
  GtkWidget *tmp_toolbar_icon;
  GtkWidget *tlbEmptyTraceset;
  GtkWidget *tlbOpenTraceset;
  GtkWidget *tlbAddTrace;
  GtkWidget *tlbRemoveTrace;
  GtkWidget *tlbSave;
  GtkWidget *tlbSaveAs;
  GtkWidget *tlbZoomIn;
  GtkWidget *tlbZoomOut;
  GtkWidget *tlbZoomExtended;
  GtkWidget *tlbGoToTime;
  GtkWidget *tlbShowTimeFrame;
  GtkWidget *tlbMoveViewerUp;
  GtkWidget *tlbMoveViewerDown;
  GtkWidget *tlbRemoveViewer;
  GtkWidget *MToolbar2;
  GtkWidget *MNotebook;
  //  GtkWidget *empty_notebook_page;
  //  GtkWidget *label1;
  GtkWidget *MStatusbar;
  GtkAccelGroup *accel_group;

  accel_group = gtk_accel_group_new ();

  MWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_size_request (MWindow, 100, 50);
  gtk_window_set_title (GTK_WINDOW (MWindow), "Main window");
  gtk_window_set_default_size (GTK_WINDOW (MWindow), 600, 400);

  MVbox = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (MVbox);
  gtk_container_add (GTK_CONTAINER (MWindow), MVbox);

  MMenuBox = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (MMenuBox);
  gtk_box_pack_start (GTK_BOX (MVbox), MMenuBox, FALSE, FALSE, 0);

  MenuMain = gtk_menu_bar_new ();
  gtk_widget_show (MenuMain);
  gtk_box_pack_start (GTK_BOX (MMenuBox), MenuMain, FALSE, FALSE, 0);

  FileMenuTitle = gtk_menu_item_new_with_mnemonic ("_File");
  gtk_widget_show (FileMenuTitle);
  gtk_container_add (GTK_CONTAINER (MenuMain), FileMenuTitle);

  FileMenuTitle_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (FileMenuTitle), FileMenuTitle_menu);

  FileMenuNewTitle = gtk_menu_item_new_with_mnemonic ("New");
  gtk_widget_show (FileMenuNewTitle);
  gtk_container_add (GTK_CONTAINER (FileMenuTitle_menu), FileMenuNewTitle);

  FileMenuNewTitle_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (FileMenuNewTitle), FileMenuNewTitle_menu);

  EmptyTraceset = gtk_menu_item_new_with_mnemonic ("Empty trace set");
  gtk_widget_show (EmptyTraceset);
  gtk_container_add (GTK_CONTAINER (FileMenuNewTitle_menu), EmptyTraceset);

  CloneTraceset = gtk_menu_item_new_with_mnemonic ("Clone trace set");
  gtk_widget_show (CloneTraceset);
  gtk_container_add (GTK_CONTAINER (FileMenuNewTitle_menu), CloneTraceset);

  FileMenuNewSep = gtk_menu_item_new ();
  gtk_widget_show (FileMenuNewSep);
  gtk_container_add (GTK_CONTAINER (FileMenuNewTitle_menu), FileMenuNewSep);
  gtk_widget_set_sensitive (FileMenuNewSep, FALSE);

  Tab = gtk_menu_item_new_with_mnemonic ("Tab");
  gtk_widget_show (Tab);
  gtk_container_add (GTK_CONTAINER (FileMenuNewTitle_menu), Tab);

  OpenTraceset = gtk_menu_item_new_with_mnemonic ("Open");
  gtk_widget_show (OpenTraceset);
  gtk_container_add (GTK_CONTAINER (FileMenuTitle_menu), OpenTraceset);

  Close = gtk_menu_item_new_with_mnemonic ("Close");
  gtk_widget_show (Close);
  gtk_container_add (GTK_CONTAINER (FileMenuTitle_menu), Close);

  CloseTab = gtk_menu_item_new_with_mnemonic ("Close Tab");
  gtk_widget_show (CloseTab);
  gtk_container_add (GTK_CONTAINER (FileMenuTitle_menu), CloseTab);

  FileMenuSeparator1 = gtk_menu_item_new ();
  gtk_widget_show (FileMenuSeparator1);
  gtk_container_add (GTK_CONTAINER (FileMenuTitle_menu), FileMenuSeparator1);
  gtk_widget_set_sensitive (FileMenuSeparator1, FALSE);

  AddTrace = gtk_menu_item_new_with_mnemonic ("Add Trace");
  gtk_widget_show (AddTrace);
  gtk_container_add (GTK_CONTAINER (FileMenuTitle_menu), AddTrace);

  RemoveTrace = gtk_menu_item_new_with_mnemonic ("Remove Trace");
  gtk_widget_show (RemoveTrace);
  gtk_container_add (GTK_CONTAINER (FileMenuTitle_menu), RemoveTrace);

  Save = gtk_menu_item_new_with_mnemonic ("Save");
  gtk_widget_show (Save);
  gtk_container_add (GTK_CONTAINER (FileMenuTitle_menu), Save);

  SaveAs = gtk_menu_item_new_with_mnemonic ("Save As");
  gtk_widget_show (SaveAs);
  gtk_container_add (GTK_CONTAINER (FileMenuTitle_menu), SaveAs);

  FileMenuSeparator2 = gtk_menu_item_new ();
  gtk_widget_show (FileMenuSeparator2);
  gtk_container_add (GTK_CONTAINER (FileMenuTitle_menu), FileMenuSeparator2);
  gtk_widget_set_sensitive (FileMenuSeparator2, FALSE);

  Quit = gtk_menu_item_new_with_mnemonic ("Quit");
  gtk_widget_show (Quit);
  gtk_container_add (GTK_CONTAINER (FileMenuTitle_menu), Quit);

  EditMenuTitle = gtk_menu_item_new_with_mnemonic ("_Edit");
  gtk_widget_show (EditMenuTitle);
  gtk_container_add (GTK_CONTAINER (MenuMain), EditMenuTitle);

  EditMenuTitle_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (EditMenuTitle), EditMenuTitle_menu);

  Cut = gtk_image_menu_item_new_from_stock ("gtk-cut", accel_group);
  gtk_widget_show (Cut);
  gtk_container_add (GTK_CONTAINER (EditMenuTitle_menu), Cut);

  Copy = gtk_image_menu_item_new_from_stock ("gtk-copy", accel_group);
  gtk_widget_show (Copy);
  gtk_container_add (GTK_CONTAINER (EditMenuTitle_menu), Copy);

  Paste = gtk_image_menu_item_new_from_stock ("gtk-paste", accel_group);
  gtk_widget_show (Paste);
  gtk_container_add (GTK_CONTAINER (EditMenuTitle_menu), Paste);

  Delete = gtk_image_menu_item_new_from_stock ("gtk-delete", accel_group);
  gtk_widget_show (Delete);
  gtk_container_add (GTK_CONTAINER (EditMenuTitle_menu), Delete);

  ViewMenuTitle = gtk_menu_item_new_with_mnemonic ("_View");
  gtk_widget_show (ViewMenuTitle);
  gtk_container_add (GTK_CONTAINER (MenuMain), ViewMenuTitle);

  ViewMenuTitle_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (ViewMenuTitle), ViewMenuTitle_menu);

  ZoomIn = gtk_menu_item_new_with_mnemonic ("Zoom in");
  gtk_widget_show (ZoomIn);
  gtk_container_add (GTK_CONTAINER (ViewMenuTitle_menu), ZoomIn);

  ZoomOut = gtk_menu_item_new_with_mnemonic ("Zoom out");
  gtk_widget_show (ZoomOut);
  gtk_container_add (GTK_CONTAINER (ViewMenuTitle_menu), ZoomOut);

  ZoomExtended = gtk_menu_item_new_with_mnemonic ("Zoom extended");
  gtk_widget_show (ZoomExtended);
  gtk_container_add (GTK_CONTAINER (ViewMenuTitle_menu), ZoomExtended);

  ViewMenuSeparator = gtk_menu_item_new ();
  gtk_widget_show (ViewMenuSeparator);
  gtk_container_add (GTK_CONTAINER (ViewMenuTitle_menu), ViewMenuSeparator);
  gtk_widget_set_sensitive (ViewMenuSeparator, FALSE);

  GoToTime = gtk_menu_item_new_with_mnemonic ("Go to time");
  gtk_widget_show (GoToTime);
  gtk_container_add (GTK_CONTAINER (ViewMenuTitle_menu), GoToTime);

  ShowTimeFrame = gtk_menu_item_new_with_mnemonic ("Show time frame");
  gtk_widget_show (ShowTimeFrame);
  gtk_container_add (GTK_CONTAINER (ViewMenuTitle_menu), ShowTimeFrame);

  ToolMenuTitle = gtk_menu_item_new_with_mnemonic ("Tools");
  gtk_widget_show (ToolMenuTitle);
  gtk_container_add (GTK_CONTAINER (MenuMain), ToolMenuTitle);

  ToolMenuTitle_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (ToolMenuTitle), ToolMenuTitle_menu);

  MoveViewerUp = gtk_menu_item_new_with_mnemonic ("Move viewer up");
  gtk_widget_show (MoveViewerUp);
  gtk_container_add (GTK_CONTAINER (ToolMenuTitle_menu), MoveViewerUp);

  MoveViewerDown = gtk_menu_item_new_with_mnemonic ("Move viewer down");
  gtk_widget_show (MoveViewerDown);
  gtk_container_add (GTK_CONTAINER (ToolMenuTitle_menu), MoveViewerDown);

  RemoveViewer = gtk_menu_item_new_with_mnemonic ("Remove viewer");
  gtk_widget_show (RemoveViewer);
  gtk_container_add (GTK_CONTAINER (ToolMenuTitle_menu), RemoveViewer);

  ToolMenuSeparator = gtk_menu_item_new ();
  gtk_widget_show (ToolMenuSeparator);
  gtk_container_add (GTK_CONTAINER (ToolMenuTitle_menu), ToolMenuSeparator);
  gtk_widget_set_sensitive (ToolMenuSeparator, FALSE);

  insert_viewer_test = gtk_menu_item_new_with_mnemonic ("Insert viewer test");
  gtk_widget_show (insert_viewer_test);
  gtk_container_add (GTK_CONTAINER (ToolMenuTitle_menu), insert_viewer_test);

  PluginMenuTitle = gtk_menu_item_new_with_mnemonic ("Plugins");
  gtk_widget_show (PluginMenuTitle);
  gtk_container_add (GTK_CONTAINER (MenuMain), PluginMenuTitle);

  PluginMenuTitle_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (PluginMenuTitle), PluginMenuTitle_menu);

  LoadModule = gtk_menu_item_new_with_mnemonic ("Load module");
  gtk_widget_show (LoadModule);
  gtk_container_add (GTK_CONTAINER (PluginMenuTitle_menu), LoadModule);

  UnloadModule = gtk_menu_item_new_with_mnemonic ("Unload module");
  gtk_widget_show (UnloadModule);
  gtk_container_add (GTK_CONTAINER (PluginMenuTitle_menu), UnloadModule);

  AddModuleSearchPath = gtk_menu_item_new_with_mnemonic ("Add module search path");
  gtk_widget_show (AddModuleSearchPath);
  gtk_container_add (GTK_CONTAINER (PluginMenuTitle_menu), AddModuleSearchPath);

  OptionMenuTitle = gtk_menu_item_new_with_mnemonic ("Options");
  gtk_widget_show (OptionMenuTitle);
  gtk_container_add (GTK_CONTAINER (MenuMain), OptionMenuTitle);

  OptionMenuTitle_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (OptionMenuTitle), OptionMenuTitle_menu);

  Color = gtk_menu_item_new_with_mnemonic ("Color");
  gtk_widget_show (Color);
  gtk_container_add (GTK_CONTAINER (OptionMenuTitle_menu), Color);

  OptMenuSeparator = gtk_menu_item_new ();
  gtk_widget_show (OptMenuSeparator);
  gtk_container_add (GTK_CONTAINER (OptionMenuTitle_menu), OptMenuSeparator);
  gtk_widget_set_sensitive (OptMenuSeparator, FALSE);

  OpenFilter = gtk_menu_item_new_with_mnemonic ("Filter");
  gtk_widget_show (OpenFilter);
  gtk_container_add (GTK_CONTAINER (OptionMenuTitle_menu), OpenFilter);

  SaveConfiguration = gtk_menu_item_new_with_mnemonic ("Save configuration");
  gtk_widget_show (SaveConfiguration);
  gtk_container_add (GTK_CONTAINER (OptionMenuTitle_menu), SaveConfiguration);

  MenuHelp = gtk_menu_bar_new ();
  gtk_widget_show (MenuHelp);
  gtk_box_pack_end (GTK_BOX (MMenuBox), MenuHelp, FALSE, FALSE, 0);

  HelpMenuTitle = gtk_menu_item_new_with_mnemonic ("_Help");
  gtk_widget_show (HelpMenuTitle);
  gtk_container_add (GTK_CONTAINER (MenuHelp), HelpMenuTitle);

  HelpMenu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (HelpMenuTitle), HelpMenu);

  Content = gtk_menu_item_new_with_mnemonic ("Content");
  gtk_widget_show (Content);
  gtk_container_add (GTK_CONTAINER (HelpMenu), Content);

  HelpmenuSeparator = gtk_menu_item_new ();
  gtk_widget_show (HelpmenuSeparator);
  gtk_container_add (GTK_CONTAINER (HelpMenu), HelpmenuSeparator);
  gtk_widget_set_sensitive (HelpmenuSeparator, FALSE);

  About = gtk_menu_item_new_with_mnemonic ("About...");
  gtk_widget_show (About);
  gtk_container_add (GTK_CONTAINER (HelpMenu), About);

  MToolbar1 = gtk_toolbar_new ();
  gtk_widget_show (MToolbar1);
  gtk_box_pack_start (GTK_BOX (MVbox), MToolbar1, FALSE, FALSE, 0);
  gtk_toolbar_set_style (GTK_TOOLBAR (MToolbar1), GTK_TOOLBAR_ICONS);

  tmp_toolbar_icon = create_pixmap (MWindow, "filenew.png");
  tlbEmptyTraceset = gtk_toolbar_append_element (GTK_TOOLBAR (MToolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "",
                                "New window with empty trace set", NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (MToolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (tlbEmptyTraceset);
  gtk_container_set_border_width (GTK_CONTAINER (tlbEmptyTraceset), 1);

  tmp_toolbar_icon = create_pixmap (MWindow, "fileopen.png");
  tlbOpenTraceset = gtk_toolbar_append_element (GTK_TOOLBAR (MToolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "",
                                "open a trace set", NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (MToolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (tlbOpenTraceset);
  gtk_container_set_border_width (GTK_CONTAINER (tlbOpenTraceset), 1);

  tmp_toolbar_icon = create_pixmap (MWindow, "edit_add_22.png");
  tlbAddTrace = gtk_toolbar_append_element (GTK_TOOLBAR (MToolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "",
                                "Add a trace ", NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (MToolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (tlbAddTrace);
  gtk_container_set_border_width (GTK_CONTAINER (tlbAddTrace), 1);

  tmp_toolbar_icon = create_pixmap (MWindow, "edit_remove_22.png");
  tlbRemoveTrace = gtk_toolbar_append_element (GTK_TOOLBAR (MToolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "",
                                "Remove a trace", NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (MToolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (tlbRemoveTrace);
  gtk_container_set_border_width (GTK_CONTAINER (tlbRemoveTrace), 1);

  tmp_toolbar_icon = create_pixmap (MWindow, "filesave.png");
  tlbSave = gtk_toolbar_append_element (GTK_TOOLBAR (MToolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "",
                                "save the current trace set", NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (MToolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (tlbSave);
  gtk_container_set_border_width (GTK_CONTAINER (tlbSave), 1);

  tmp_toolbar_icon = create_pixmap (MWindow, "filesaveas.png");
  tlbSaveAs = gtk_toolbar_append_element (GTK_TOOLBAR (MToolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "",
                                "save as ", NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (MToolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (tlbSaveAs);
  gtk_container_set_border_width (GTK_CONTAINER (tlbSaveAs), 1);

  gtk_toolbar_append_space (GTK_TOOLBAR (MToolbar1));

  tmp_toolbar_icon = create_pixmap (MWindow, "stock_zoom_in_24.png");
  tlbZoomIn = gtk_toolbar_append_element (GTK_TOOLBAR (MToolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "",
                                "Zoom in", NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (MToolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (tlbZoomIn);
  gtk_container_set_border_width (GTK_CONTAINER (tlbZoomIn), 1);

  tmp_toolbar_icon = create_pixmap (MWindow, "stock_zoom_out_24.png");
  tlbZoomOut = gtk_toolbar_append_element (GTK_TOOLBAR (MToolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "",
                                "Zoom out", NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (MToolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (tlbZoomOut);
  gtk_container_set_border_width (GTK_CONTAINER (tlbZoomOut), 1);

  tmp_toolbar_icon = create_pixmap (MWindow, "stock_zoom_fit_24.png");
  tlbZoomExtended = gtk_toolbar_append_element (GTK_TOOLBAR (MToolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "",
                                "Zoom extended", NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (MToolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (tlbZoomExtended);
  gtk_container_set_border_width (GTK_CONTAINER (tlbZoomExtended), 1);

  tmp_toolbar_icon = create_pixmap (MWindow, "gtk-jump-to.png");
  tlbGoToTime = gtk_toolbar_append_element (GTK_TOOLBAR (MToolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "",
                                "Go to time", NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (MToolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (tlbGoToTime);
  gtk_container_set_border_width (GTK_CONTAINER (tlbGoToTime), 1);

  tmp_toolbar_icon = create_pixmap (MWindow, "mini-display.xpm");
  tlbShowTimeFrame = gtk_toolbar_append_element (GTK_TOOLBAR (MToolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "",
                                "Show time frame", NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (MToolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (tlbShowTimeFrame);
  gtk_container_set_border_width (GTK_CONTAINER (tlbShowTimeFrame), 1);

  gtk_toolbar_append_space (GTK_TOOLBAR (MToolbar1));

  tmp_toolbar_icon = create_pixmap (MWindow, "1uparrow.png");
  tlbMoveViewerUp = gtk_toolbar_append_element (GTK_TOOLBAR (MToolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "",
                                "Move up current viewer", NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (MToolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (tlbMoveViewerUp);
  gtk_container_set_border_width (GTK_CONTAINER (tlbMoveViewerUp), 1);

  tmp_toolbar_icon = create_pixmap (MWindow, "1downarrow.png");
  tlbMoveViewerDown = gtk_toolbar_append_element (GTK_TOOLBAR (MToolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "",
                                "Move down current viewer", NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (MToolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (tlbMoveViewerDown);
  gtk_container_set_border_width (GTK_CONTAINER (tlbMoveViewerDown), 1);

  tmp_toolbar_icon = create_pixmap (MWindow, "remove.png");
  tlbRemoveViewer = gtk_toolbar_append_element (GTK_TOOLBAR (MToolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                "",
                                "Delete current viewer", NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_label_set_use_underline (GTK_LABEL (((GtkToolbarChild*) (g_list_last (GTK_TOOLBAR (MToolbar1)->children)->data))->label), TRUE);
  gtk_widget_show (tlbRemoveViewer);
  gtk_container_set_border_width (GTK_CONTAINER (tlbRemoveViewer), 1);

  MToolbar2 = gtk_toolbar_new ();
  gtk_widget_show (MToolbar2);
  gtk_box_pack_start (GTK_BOX (MVbox), MToolbar2, FALSE, FALSE, 0);
  gtk_toolbar_set_style (GTK_TOOLBAR (MToolbar2), GTK_TOOLBAR_BOTH);

  MNotebook = gtk_notebook_new ();
  gtk_widget_show (MNotebook);
  gtk_box_pack_start (GTK_BOX (MVbox), MNotebook, TRUE, TRUE, 0);
/*
  empty_notebook_page = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (empty_notebook_page);
  gtk_container_add (GTK_CONTAINER (MNotebook), empty_notebook_page);

  label1 = gtk_label_new ("");
  gtk_widget_show (label1);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (MNotebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (MNotebook), 0), label1);
  gtk_label_set_justify (GTK_LABEL (label1), GTK_JUSTIFY_LEFT);
*/
  MStatusbar = gtk_statusbar_new ();
  gtk_widget_show (MStatusbar);
  gtk_box_pack_start (GTK_BOX (MVbox), MStatusbar, FALSE, FALSE, 0);

  g_signal_connect ((gpointer) MWindow, "destroy",
                    G_CALLBACK (on_MWindow_destroy),
                    NULL);
  g_signal_connect ((gpointer) EmptyTraceset, "activate",
                    G_CALLBACK (on_empty_traceset_activate),
                    NULL);
  g_signal_connect ((gpointer) CloneTraceset, "activate",
                    G_CALLBACK (on_clone_traceset_activate),
                    NULL);
  g_signal_connect ((gpointer) Tab, "activate",
                    G_CALLBACK (on_tab_activate),
                    NULL);
  g_signal_connect ((gpointer) OpenTraceset, "activate",
                    G_CALLBACK (on_open_activate),
                    NULL);
  g_signal_connect ((gpointer) Close, "activate",
                    G_CALLBACK (on_close_activate),
                    NULL);
  g_signal_connect ((gpointer) CloseTab, "activate",
                    G_CALLBACK (on_close_tab_activate),
                    NULL);
  g_signal_connect ((gpointer) AddTrace, "activate",
                    G_CALLBACK (on_add_trace_activate),
                    NULL);
  g_signal_connect ((gpointer) RemoveTrace, "activate",
                    G_CALLBACK (on_remove_trace_activate),
                    NULL);
  g_signal_connect ((gpointer) Save, "activate",
                    G_CALLBACK (on_save_activate),
                    NULL);
  g_signal_connect ((gpointer) SaveAs, "activate",
                    G_CALLBACK (on_save_as_activate),
                    NULL);
  g_signal_connect ((gpointer) Quit, "activate",
                    G_CALLBACK (on_quit_activate),
                    NULL);
  g_signal_connect ((gpointer) Cut, "activate",
                    G_CALLBACK (on_cut_activate),
                    NULL);
  g_signal_connect ((gpointer) Copy, "activate",
                    G_CALLBACK (on_copy_activate),
                    NULL);
  g_signal_connect ((gpointer) Paste, "activate",
                    G_CALLBACK (on_paste_activate),
                    NULL);
  g_signal_connect ((gpointer) Delete, "activate",
                    G_CALLBACK (on_delete_activate),
                    NULL);
  g_signal_connect ((gpointer) ZoomIn, "activate",
                    G_CALLBACK (on_zoom_in_activate),
                    NULL);
  g_signal_connect ((gpointer) ZoomOut, "activate",
                    G_CALLBACK (on_zoom_out_activate),
                    NULL);
  g_signal_connect ((gpointer) ZoomExtended, "activate",
                    G_CALLBACK (on_zoom_extended_activate),
                    NULL);
  g_signal_connect ((gpointer) GoToTime, "activate",
                    G_CALLBACK (on_go_to_time_activate),
                    NULL);
  g_signal_connect ((gpointer) ShowTimeFrame, "activate",
                    G_CALLBACK (on_show_time_frame_activate),
                    NULL);
  g_signal_connect ((gpointer) MoveViewerUp, "activate",
                    G_CALLBACK (on_move_viewer_up_activate),
                    NULL);
  g_signal_connect ((gpointer) MoveViewerDown, "activate",
                    G_CALLBACK (on_move_viewer_down_activate),
                    NULL);
  g_signal_connect ((gpointer) RemoveViewer, "activate",
                    G_CALLBACK (on_remove_viewer_activate),
                    NULL);
  g_signal_connect ((gpointer) insert_viewer_test, "activate",
                    G_CALLBACK (on_insert_viewer_test_activate),
                    NULL);
  g_signal_connect ((gpointer) LoadModule, "activate",
                    G_CALLBACK (on_load_module_activate),
                    NULL);
  g_signal_connect ((gpointer) UnloadModule, "activate",
                    G_CALLBACK (on_unload_module_activate),
                    NULL);
  g_signal_connect ((gpointer) AddModuleSearchPath, "activate",
                    G_CALLBACK (on_add_module_search_path_activate),
                    NULL);
  g_signal_connect ((gpointer) Color, "activate",
                    G_CALLBACK (on_color_activate),
                    NULL);
  g_signal_connect ((gpointer) OpenFilter, "activate",
                    G_CALLBACK (on_filter_activate),
                    NULL);
  g_signal_connect ((gpointer) SaveConfiguration, "activate",
                    G_CALLBACK (on_save_configuration_activate),
                    NULL);
  g_signal_connect ((gpointer) Content, "activate",
                    G_CALLBACK (on_content_activate),
                    NULL);
  g_signal_connect ((gpointer) About, "activate",
                    G_CALLBACK (on_about_activate),
                    NULL);
  g_signal_connect ((gpointer) tlbEmptyTraceset, "clicked",
                    G_CALLBACK (on_button_new_clicked),
                    NULL);
  g_signal_connect ((gpointer) tlbOpenTraceset, "clicked",
                    G_CALLBACK (on_button_open_clicked),
                    NULL);
  g_signal_connect ((gpointer) tlbAddTrace, "clicked",
                    G_CALLBACK (on_button_add_trace_clicked),
                    NULL);
  g_signal_connect ((gpointer) tlbRemoveTrace, "clicked",
                    G_CALLBACK (on_button_remove_trace_clicked),
                    NULL);
  g_signal_connect ((gpointer) tlbSave, "clicked",
                    G_CALLBACK (on_button_save_clicked),
                    NULL);
  g_signal_connect ((gpointer) tlbSaveAs, "clicked",
                    G_CALLBACK (on_button_save_as_clicked),
                    NULL);
  g_signal_connect ((gpointer) tlbZoomIn, "clicked",
                    G_CALLBACK (on_button_zoom_in_clicked),
                    NULL);
  g_signal_connect ((gpointer) tlbZoomOut, "clicked",
                    G_CALLBACK (on_button_zoom_out_clicked),
                    NULL);
  g_signal_connect ((gpointer) tlbZoomExtended, "clicked",
                    G_CALLBACK (on_button_zoom_extended_clicked),
                    NULL);
  g_signal_connect ((gpointer) tlbGoToTime, "clicked",
                    G_CALLBACK (on_button_go_to_time_clicked),
                    NULL);
  g_signal_connect ((gpointer) tlbShowTimeFrame, "clicked",
                    G_CALLBACK (on_button_show_time_frame_clicked),
                    NULL);
  g_signal_connect ((gpointer) tlbMoveViewerUp, "clicked",
                    G_CALLBACK (on_button_move_up_clicked),
                    NULL);
  g_signal_connect ((gpointer) tlbMoveViewerDown, "clicked",
                    G_CALLBACK (on_button_move_down_clicked),
                    NULL);
  g_signal_connect ((gpointer) tlbRemoveViewer, "clicked",
                    G_CALLBACK (on_button_delete_viewer_clicked),
                    NULL);
  g_signal_connect ((gpointer) MNotebook, "switch_page",
                    G_CALLBACK (on_MNotebook_switch_page),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (MWindow, MWindow, "MWindow");
  GLADE_HOOKUP_OBJECT (MWindow, MVbox, "MVbox");
  GLADE_HOOKUP_OBJECT (MWindow, MMenuBox, "MMenuBox");
  GLADE_HOOKUP_OBJECT (MWindow, MenuMain, "MenuMain");
  GLADE_HOOKUP_OBJECT (MWindow, FileMenuTitle, "FileMenuTitle");
  GLADE_HOOKUP_OBJECT (MWindow, FileMenuTitle_menu, "FileMenuTitle_menu");
  GLADE_HOOKUP_OBJECT (MWindow, FileMenuNewTitle, "FileMenuNewTitle");
  GLADE_HOOKUP_OBJECT (MWindow, FileMenuNewTitle_menu, "FileMenuNewTitle_menu");
  GLADE_HOOKUP_OBJECT (MWindow, EmptyTraceset, "EmptyTraceset");
  GLADE_HOOKUP_OBJECT (MWindow, CloneTraceset, "CloneTraceset");
  GLADE_HOOKUP_OBJECT (MWindow, FileMenuNewSep, "FileMenuNewSep");
  GLADE_HOOKUP_OBJECT (MWindow, Tab, "Tab");
  GLADE_HOOKUP_OBJECT (MWindow, OpenTraceset, "OpenTraceset");
  GLADE_HOOKUP_OBJECT (MWindow, Close, "Close");
  GLADE_HOOKUP_OBJECT (MWindow, CloseTab, "CloseTab");
  GLADE_HOOKUP_OBJECT (MWindow, FileMenuSeparator1, "FileMenuSeparator1");
  GLADE_HOOKUP_OBJECT (MWindow, AddTrace, "AddTrace");
  GLADE_HOOKUP_OBJECT (MWindow, RemoveTrace, "RemoveTrace");
  GLADE_HOOKUP_OBJECT (MWindow, Save, "Save");
  GLADE_HOOKUP_OBJECT (MWindow, SaveAs, "SaveAs");
  GLADE_HOOKUP_OBJECT (MWindow, FileMenuSeparator2, "FileMenuSeparator2");
  GLADE_HOOKUP_OBJECT (MWindow, Quit, "Quit");
  GLADE_HOOKUP_OBJECT (MWindow, EditMenuTitle, "EditMenuTitle");
  GLADE_HOOKUP_OBJECT (MWindow, EditMenuTitle_menu, "EditMenuTitle_menu");
  GLADE_HOOKUP_OBJECT (MWindow, Cut, "Cut");
  GLADE_HOOKUP_OBJECT (MWindow, Copy, "Copy");
  GLADE_HOOKUP_OBJECT (MWindow, Paste, "Paste");
  GLADE_HOOKUP_OBJECT (MWindow, Delete, "Delete");
  GLADE_HOOKUP_OBJECT (MWindow, ViewMenuTitle, "ViewMenuTitle");
  GLADE_HOOKUP_OBJECT (MWindow, ViewMenuTitle_menu, "ViewMenuTitle_menu");
  GLADE_HOOKUP_OBJECT (MWindow, ZoomIn, "ZoomIn");
  GLADE_HOOKUP_OBJECT (MWindow, ZoomOut, "ZoomOut");
  GLADE_HOOKUP_OBJECT (MWindow, ZoomExtended, "ZoomExtended");
  GLADE_HOOKUP_OBJECT (MWindow, ViewMenuSeparator, "ViewMenuSeparator");
  GLADE_HOOKUP_OBJECT (MWindow, GoToTime, "GoToTime");
  GLADE_HOOKUP_OBJECT (MWindow, ShowTimeFrame, "ShowTimeFrame");
  GLADE_HOOKUP_OBJECT (MWindow, ToolMenuTitle, "ToolMenuTitle");
  GLADE_HOOKUP_OBJECT (MWindow, ToolMenuTitle_menu, "ToolMenuTitle_menu");
  GLADE_HOOKUP_OBJECT (MWindow, MoveViewerUp, "MoveViewerUp");
  GLADE_HOOKUP_OBJECT (MWindow, MoveViewerDown, "MoveViewerDown");
  GLADE_HOOKUP_OBJECT (MWindow, RemoveViewer, "RemoveViewer");
  GLADE_HOOKUP_OBJECT (MWindow, ToolMenuSeparator, "ToolMenuSeparator");
  GLADE_HOOKUP_OBJECT (MWindow, insert_viewer_test, "insert_viewer_test");
  GLADE_HOOKUP_OBJECT (MWindow, PluginMenuTitle, "PluginMenuTitle");
  GLADE_HOOKUP_OBJECT (MWindow, PluginMenuTitle_menu, "PluginMenuTitle_menu");
  GLADE_HOOKUP_OBJECT (MWindow, LoadModule, "LoadModule");
  GLADE_HOOKUP_OBJECT (MWindow, UnloadModule, "UnloadModule");
  GLADE_HOOKUP_OBJECT (MWindow, AddModuleSearchPath, "AddModuleSearchPath");
  GLADE_HOOKUP_OBJECT (MWindow, OptionMenuTitle, "OptionMenuTitle");
  GLADE_HOOKUP_OBJECT (MWindow, OptionMenuTitle_menu, "OptionMenuTitle_menu");
  GLADE_HOOKUP_OBJECT (MWindow, Color, "Color");
  GLADE_HOOKUP_OBJECT (MWindow, OptMenuSeparator, "OptMenuSeparator");
  GLADE_HOOKUP_OBJECT (MWindow, OpenFilter, "OpenFilter");
  GLADE_HOOKUP_OBJECT (MWindow, SaveConfiguration, "SaveConfiguration");
  GLADE_HOOKUP_OBJECT (MWindow, MenuHelp, "MenuHelp");
  GLADE_HOOKUP_OBJECT (MWindow, HelpMenuTitle, "HelpMenuTitle");
  GLADE_HOOKUP_OBJECT (MWindow, HelpMenu, "HelpMenu");
  GLADE_HOOKUP_OBJECT (MWindow, Content, "Content");
  GLADE_HOOKUP_OBJECT (MWindow, HelpmenuSeparator, "HelpmenuSeparator");
  GLADE_HOOKUP_OBJECT (MWindow, About, "About");
  GLADE_HOOKUP_OBJECT (MWindow, MToolbar1, "MToolbar1");
  GLADE_HOOKUP_OBJECT (MWindow, tlbEmptyTraceset, "tlbEmptyTraceset");
  GLADE_HOOKUP_OBJECT (MWindow, tlbOpenTraceset, "tlbOpenTraceset");
  GLADE_HOOKUP_OBJECT (MWindow, tlbAddTrace, "tlbAddTrace");
  GLADE_HOOKUP_OBJECT (MWindow, tlbRemoveTrace, "tlbRemoveTrace");
  GLADE_HOOKUP_OBJECT (MWindow, tlbSave, "tlbSave");
  GLADE_HOOKUP_OBJECT (MWindow, tlbSaveAs, "tlbSaveAs");
  GLADE_HOOKUP_OBJECT (MWindow, tlbZoomIn, "tlbZoomIn");
  GLADE_HOOKUP_OBJECT (MWindow, tlbZoomOut, "tlbZoomOut");
  GLADE_HOOKUP_OBJECT (MWindow, tlbZoomExtended, "tlbZoomExtended");
  GLADE_HOOKUP_OBJECT (MWindow, tlbGoToTime, "tlbGoToTime");
  GLADE_HOOKUP_OBJECT (MWindow, tlbShowTimeFrame, "tlbShowTimeFrame");
  GLADE_HOOKUP_OBJECT (MWindow, tlbMoveViewerUp, "tlbMoveViewerUp");
  GLADE_HOOKUP_OBJECT (MWindow, tlbMoveViewerDown, "tlbMoveViewerDown");
  GLADE_HOOKUP_OBJECT (MWindow, tlbRemoveViewer, "tlbRemoveViewer");
  GLADE_HOOKUP_OBJECT (MWindow, MToolbar2, "MToolbar2");
  GLADE_HOOKUP_OBJECT (MWindow, MNotebook, "MNotebook");
  //  GLADE_HOOKUP_OBJECT (MWindow, label1, "label1");
  GLADE_HOOKUP_OBJECT (MWindow, MStatusbar, "MStatusbar");

  gtk_window_add_accel_group (GTK_WINDOW (MWindow), accel_group);

  return MWindow;
}

