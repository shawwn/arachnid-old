
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
	end

	io.write( "\n" )

	print( string.format( "\tcamera XYZ:" ), World.Cameras["CamXYZ"].Name )
	print( string.format( "\tcamera ABC:" ), World.Cameras["CamABC"].Name )

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
			print( string.format( "\t\t\t%s:", k ), v )
		end
	end

	io.write( "\n" )

	print( string.format( "\tentity 'Head':" ), World.Entities["Head"].Name )

	io.write( "\n" )
end

