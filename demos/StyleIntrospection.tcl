###############################################################################
## StyleIntrospection.tcl:
## This demo shows information about the Gtk/GNOME style currently in use.
##
###############################################################################
if {[catch {package require Ttk}]} {
  package require tile
}
ttk::setTheme tilegtk

set win {}
label $win.lbl_option   -text Option:
label $win.lbl_1 -text NORMAL
label $win.lbl_2 -text PRELIGHT
label $win.lbl_3 -text ACTIVE
label $win.lbl_4 -text SELECTED
label $win.lbl_5 -text INSENSITIVE
grid $win.lbl_option $win.lbl_1 $win.lbl_2 $win.lbl_3 $win.lbl_4 $win.lbl_5 \
     -sticky snew -padx 2 -pady 2
set row 1
foreach base {fg bg base text light mid dark text_aa} {
  set col 0
  label $win.$base -text "$base:" -anchor w
  grid  $win.$base -row $row -column $col
  foreach state {NORMAL PRELIGHT ACTIVE SELECTED INSENSITIVE} {
    set colour [ttk::theme::tilegtk::currentThemeColour ${base}($state)]
    label $win.${base}_$state -background $colour \
      -relief raised -borderwidth 1
    label $win.${base}_${state}_value -text $colour
    grid $win.${base}_$state -row $row -column [incr col] \
         -sticky snew -padx 2 -pady 2
    grid $win.${base}_${state}_value -row [expr {$row+1}] -column $col \
         -sticky snew -padx 2 -pady 2
  }
  incr row 2
}
