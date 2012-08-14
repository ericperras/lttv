# -* sh *-

# This is a simple script that starts lttv with default GUI modules
# Mathieu Desnoyers 15-09-2005

LTTV_CMD=`echo $0 | sed 's/-gui$//'`

$LTTV_CMD.real -m lttvwindow -m guievents -m guicontrolflow  \
     $*
#-m guifilter -m guistatistics -m guitracecontrol -m resourceview
