timePassed = 0
function Update(timeDelta)

	-- Only the server creates crates.
	if(IsClient() == 0) then
		timePassed = timePassed + timeDelta
		
		if(timePassed > (300000)) then
			x = math.random(300) - 150
			y = math.random(300) - 150
			z = 50
			SpawnCrate(x, y, z)
			timePassed = 0
		
			tankCount = GetTankCount()
			
			for i = 1, tankCount do
				SendTankMessage(GetTank(i), {MessageType = "CrateDrop", pos_x = x, pos_y = y, pos_z = z})
			end
		end
	end
end
