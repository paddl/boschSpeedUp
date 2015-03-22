//1 kilometer = 0.621371192 miles
k=0.621371192;


D_cm=29*2.54
U_cm=D_cm*%pi
v_kmh=[0:50]'; //km/h
v_mph=v_kmh*k; //miles/h

v_cms = v_kmh*1e3*100/3600;
f_hz=v_cms/U_cm;




scf(1)
clf(1)
plot2d(v_kmh,[f_hz f_hz*k])
xtitle('Speedsensor','km/h','Hz')

