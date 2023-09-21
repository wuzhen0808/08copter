##
# Start flight gear view to listen the log output from jsbsim.
# You need config the output in the test script of JSBSim. 
# Some thing like below.
#  <output name="localhost" type="FLIGHTGEAR" port="5500" protocol="TCP" rate="10"/>
##
#export A8_HOME=parent folder of this file located.
export A8_AIRCRAFT=arducopter
export A8_JSBSIM_OUTPUT_PART=5500
#export A8_AIRCRAFT=F450
fgfs \
	--fg-root=$FG_HOME/data \
	--language=en \
	--fdm=external \
	--native-fdm=socket,in,10,localhost,$A8_JSBSIM_OUTPUT_PART,tcp \
	--airport=PHBK \
	--timeofday=noon \
	--fg-aircraft="$A8_HOME/simulation/aircraft" \
	--aircraft=$A8_AIRCRAFT \
	--geometry=1920x1080 \
    --bpp=32 \
    --disable-hud-3d \
    --disable-horizon-effect \
    --disable-fullscreen \
    --disable-random-objects \
    --disable-ai-models \
    --fog-disable \
    --disable-specular-highlight \
    --disable-anti-alias-hud \
    --wind=0@0 \
#    --disable-sound \