#Author: Muaz Latif
def main():
	##Estes Alpha III
	length = 0.311 		#meters
	diam = 0.0241 		#meters
	bodyArea = 0.000506	#square meters cross section
	bodyCD = 0.45		#dimensionless
	finArea = 0.00496	#square meters
	cdFin = 0.01		#dimensionless
	w_mass = 0.0340		#kilogram without engine
	engine1 = 0.0242	#kilogram initial engine mass
	engine2 = 0.0094	#kilogram final engine mass

	##Thrust Curve
	total_impulse = 8.82	#newton seconds(area under curve)
	peak_thrust = 14.09		#newton
	average_thrust = 4.74	#newton
	burn_time = 1.86		#seconds

	##Initial conditions
	t = 0				#time
	s = 0				#height
	v = 0				#velocity
	a = 0				#acceleration
	F = 0				#total force not including gravity
	m = 0.0582			#mass
	i = 0				#counter for Ft, each increment is .1 sec
	##Set values
	dt = 0.1
	Rho = 1.293 		#kg/m^3
	g = 9.80665			#gravity
	Ft = [6, 13, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0] #values estimated from graph

	while v >= 0: 										#quits when v becomes negative
		Fd_body = bodyCD * Rho * bodyArea * (v**2) * .5	#Force of drag of body
		Fd_fins = cdFin * Rho * finArea * (v**2) * .5	#Force of drag of fin
		Fg = m*g										#Force of gravity
		F = Ft[i] - (Fd_body + Fd_fins + Fg)			#resolve forces
		a = F/m 										#acceleration
		dv = a * dt 									#velocity changes
		v = v + dv										#velocity
		ds = v*dt										#change in distance
		s = s+ds 										#current distance
		m = m - (0.0001644*Ft[i])						#time step applied
		t = t + dt 										#change in time
		if i < 18: 										#time counter for Ft
			i = i + 1
		print("\n time: ", t,
            "distance: ", s,
            "velocity: ", v,
            "acceleration: ", a,
            "total force: ", F,
            "mass: ", m)

main()