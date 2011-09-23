assert( Config ~= nil )
assert( Config.Screen ~= nil )

_G["Screen"] = Config.Screen

print( string.format( "Resolution: (%d, %d)", Screen.Width, Screen.Height ) )


function vec3(x,y,z)
	return string.format( "(%g, %g, %g)", x, y, z );
end

