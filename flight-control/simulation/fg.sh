##
# Start flight gear view to listen the log output from jsbsim.
# You need config the output in the test script of JSBSim. 
# Some thing like below.
#  <output name="localhost" type="FLIGHTGEAR" port="5501" protocol="TCP" rate="10"/>
##
#export A8_HOME=parent folder of this file located.
export A8_AIRCRAFT=qc2309
export A8_JSBSIM_OUTPUT_PORT=5501
#export A8_AIRCRAFT=F450
fgfs \
	--fg-root=$FG_HOME/data \
	--language=en \
	--fdm=external \
	--native-fdm=socket,in,10,localhost,$A8_JSBSIM_OUTPUT_PORT,tcp \
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
    --time-match-local \
    --lon=121 \
    --lat=31 \
    --disable-sound \
#	--airport=PHBK \
#	--timeofday=noon \
