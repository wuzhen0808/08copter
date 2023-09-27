#!/bin/tcsh
# prep_plot is a program provided by JSBSim.
# you need the source code to compile it.

foreach file (data_log/*.csv)

  echo "process file:$file"  
  set name = `basename $file`
  set rpath = data_plot/${name}.r.xml
  if (-f $rpath) prep_plot $file --plot=$rpath | gnuplot    
  
end
gs -q -dNOPAUSE -dBATCH -sDEVICE=pdfwrite -sOutputFile=data_plot/output.pdf data_log/*.ps