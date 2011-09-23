
assert( World ~= nil )
assert( World.Cameras ~= nil )
assert( World.Entities ~= nil )


print( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" )
print( "~ Cameras" )
print( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" )
do
	print( string.format( "\t%d cameras", #World.Cameras ) )
	for i,camera in ipairs( World.Cameras ) do
		print( string.format( "\t\tcamera #%d: ", i ), camera.Name )
		for k,v in pairs( camera ) do
			print( string.format( "\t\t\t%s:\t", k ), v )
		end
	end
	io.write( "\n" )
end


print( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" )
print( "~ Entities" )
print( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" )
do
	print( string.format( "\t%d entities", #World.Entities ) )
	for i,entity in ipairs( World.Entities ) do
		print( string.format( "\t\tentity #%d: ", i ) )
		for k,v in pairs( entity ) do
			print( string.format( "\t\t\t%s:\t", k ), v )
		end
	end
	io.write( "\n" )
end

