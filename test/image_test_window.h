// generated by Fast Light User Interface Designer (fluid) version 1.0104

#ifndef image_test_window_h
#define image_test_window_h
#include <FL/Fl.H>
#include "image_test.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Menu_Bar.H>

class image_test_window {
public:
  image_test_window();
  Fl_Double_Window *main_window;
  image_draw_area *draw_area;
private:
  inline void cb_Read_i(Fl_Button*, void*);
  static void cb_Read(Fl_Button*, void*);
  inline void cb_Read1_i(Fl_Button*, void*);
  static void cb_Read1(Fl_Button*, void*);
  inline void cb_Write_i(Fl_Button*, void*);
  static void cb_Write(Fl_Button*, void*);
  inline void cb_Write1_i(Fl_Button*, void*);
  static void cb_Write1(Fl_Button*, void*);
  inline void cb_Read2_i(Fl_Button*, void*);
  static void cb_Read2(Fl_Button*, void*);
  inline void cb_Write2_i(Fl_Button*, void*);
  static void cb_Write2(Fl_Button*, void*);
  inline void cb_Read3_i(Fl_Button*, void*);
  static void cb_Read3(Fl_Button*, void*);
  inline void cb_Write3_i(Fl_Button*, void*);
  static void cb_Write3(Fl_Button*, void*);
  inline void cb_Read4_i(Fl_Button*, void*);
  static void cb_Read4(Fl_Button*, void*);
  inline void cb_Write4_i(Fl_Button*, void*);
  static void cb_Write4(Fl_Button*, void*);
  static Fl_Menu_Item menu_[];
  inline void cb_Euclidean_i(Fl_Menu_*, void*);
  static void cb_Euclidean(Fl_Menu_*, void*);
public:
  void show( );
};
#endif
