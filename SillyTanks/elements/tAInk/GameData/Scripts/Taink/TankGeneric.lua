require "GameData\\Scripts\\VectorMath"
-- //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cnt = 0
run_cnt = 0
squid_id = 0
my_id = 0

angle = 0

DEBUG = 0
CTF = 0
friendlyFlagPos 	= {0, 0, 0}
enemyFlagPos 		= {0, 0, 0}
hasFlag				= false

curPos 			= {0, 0, 0}
lastPos 		= {0, 0, 0}
stuckCnt		= 0
stuckSign		= 1

STRATEGY_EXPLORE		=	0
STRATEGY_HUNT			=	1
STRATEGY_RETREAT		=	2

currentStrategy 		= 	STRATEGY_EXPLORE
currentStrategyTarget	= 	nil

function SwitchStrategy(newStrategy, newStrategyTarget)
	-- Can't hunt or retreat from ourselves
	if(newStrategy == STRATEGY_HUNT or newStrategy == STRATEGY_RETREAT) then
		if(newStrategyTarget == my_id) then
			return
		end
	end
	
	currentStrategy = newStrategy
	currentStrategyTarget = newStrategyTarget
	
	print("Tank %d switching to strategy %d\n", GetTank(), newStrategy)
end

function Setup()
	my_id = GetTank()
	lastPos = {GetTankPosition(my_id)}
end

function PickTarget()
	if(GetTankCount() == 1) then
		return 0
	end
	
	if((run_cnt % 100) == 0 or not target_id) then		
		repeat
			target_id = GetTank(math.random(1, GetTankCount()))
		until(target_id ~= my_id)		
		-- print("[%s] New Target Picked: %s [%d], Current Score: %d\n", GetTankName(my_id), GetTankName(target_id), target_id, GetTankScore(target_id))
	end
	
	return target_id
end

function Debug()
	-- SetCannonSpeed(0.5)
	FireWeapon(0)
end

function AimAndFire(target)	
	if(not target) then
		return
	end
	
	-- Get relevant info (target pos, dir, cannon dir, etc)	
	local myTankPos		= {GetTankPosition(my_id)}
	local myTankDir		= {GetTankDirection(my_id)}
	local myCannonDir 	= {GetCannonDirection(my_id)}
	local targetPos 	= {GetTankPosition(target)}
	local targetDir		= {GetTankDirection(target)}
					
	-- Normalize the vector
	local vec = Vec3:Subtract(targetPos, myTankPos)
	local vec = Vec3:Normalize(vec)
	
	-- Calc dot and cross between my facing and target, 
	local cross = Vec3:Cross(vec, myTankDir)
	local dot = Vec3:Dot(vec, myTankDir)
				
	-- Set the cannon toward the target
	if(cross[3] >= 0) then
		SetCannonAngle(math.acos(dot), 1.0)
	else
		SetCannonAngle(-math.acos(dot), 1.0)
	end
	
	-- Fire cannon if we're sorta pointing at the target
	if(Vec3:Dot(vec, myCannonDir) > math.cos(math.rad(2.5))) then
		if(GetWeaponAmmo(1) > 0) then
			FireWeapon(1)
		else
			FireWeapon(0)
		end
	end	
end

function ProcessMessages(messages)
	if(messages.count > 0) then
		for i = 1, messages.count do
			message = messages[i]			
			print("Message Received: " .. message.MessageType .. "\n")
			
			if (message.MessageType == "GotHit") then			-- We got hit!  Holy crap, run away!
				SwitchStrategy(STRATEGY_RETREAT, message.Source)
			elseif (message.MessageType == "HitTarget") then	-- We hit someone!  HUNT THEM DOWN
				if(currentStrategy ~= STRATEGY_HUNT) then
					SwitchStrategy(STRATEGY_HUNT, message.Target)
				end
			end
		end
	end
end

function StuckCheck()
	lastPos = curPos
	curPos = {GetTankPosition(my_id)}
	
	-- print("last vs cur:  (%0.2f, %0.2f, %0.2f) vs (%0.2f, %0.2f, %0.2f)\n", lastPos[1], lastPos[2], lastPos[3], curPos[1], curPos[2], curPos[3])
	-- print("Stuck distance:  %f\n", Vec3:Distance(lastPos, curPos)) 
	
	if(stuckCnt < 0) then
		stuckCnt = stuckCnt + 1
		return
	end
	
	if(Vec3:Distance(lastPos, curPos) < 2) then
		if(stuckCnt >= 0) then
			stuckCnt = stuckCnt + 1			
			if(stuckCnt == 10) then
				stuckCnt = (-stuckCnt)				
				stuckSign = (-stuckSign)
				
				SetLeftTrackSpeed(stuckSign * 0.5)
				SetRightTrackSpeed(stuckSign * 1.0)
				return true
			end			
		end
	else
		stuckCnt = 0
	end
	
	return false
end

function Explore()
	target = PickTarget()
	
	if(target ~= nil) then
		SwitchStrategy(STRATEGY_HUNT, target)
		return
	end
end

function Hunt()
	myTankPos		= {GetTankPosition(my_id)}
	myTankDir		= {GetTankDirection(my_id)}
	myCannonDir 	= {GetCannonDirection(my_id)}
	
	-- Let's first see if any tanks are currently pointing at me.
	-- If so, turn and f'n book for it.
	--[[
	for i = 1, GetTankCount() do
		if(GetTank(i) ~= my_id) then
			tankCannonDir = {GetCannonDirection(GetTank(i))}
			if(Vec3:Dot(tankCannonDir, Vec3:Normalize(Vec3:Subtract(myTankPos, {GetTankPosition(GetTank(i))}))) > math.cos(math.rad(15))) then		
				SwitchStrategy(STRATEGY_RETREAT,  GetTank(i))
				break
			end
		end
	end
	]]
	
	run_cnt = run_cnt + 1	
	
	if(currentStrategyTarget) then
		-- Get relevant info (target pos, dir, cannon dir, etc)	
		targetPos 		= {GetTankPosition(currentStrategyTarget)}
		targetDir		= {GetTankDirection(currentStrategyTarget)}
		targetCannonDir	= {GetCannonDirection(currentStrategyTarget)}
						
		-- Normalize the vector
		vec = Vec3:Subtract(targetPos, myTankPos)
		vec = Vec3:Normalize(vec)
		
		-- Calc dot and cross between my facing and target, 
		cross = Vec3:Cross(vec, myTankDir)
		dot = Vec3:Dot(vec, myTankDir)
				
		if(cross[3] >= 0) then
			SetLeftTrackSpeed(1.0)
			SetRightTrackSpeed(0.65)
		else
			SetLeftTrackSpeed(0.65)
			SetRightTrackSpeed(1.0)
		end
		
		AimAndFire(currentStrategyTarget)	
	else		
		SetLeftTrackSpeed(1.0)
		SetRightTrackSpeed(-1.0)
	end
end

function Retreat()
	if(currentStrategyTarget ~= nil) then
		
		local myTankPos			= {GetTankPosition(my_id)}
		local myTankDir			= {GetTankDirection(my_id)}
		local myCannonDir 		= {GetCannonDirection(my_id)}
		
		local targetPos 		= {GetTankPosition(currentStrategyTarget)}
		local targetDir			= {GetTankDirection(currentStrategyTarget)}
		local targetCannonDir	= {GetCannonDirection(currentStrategyTarget)}
		
		if(Vec3:Dot(targetCannonDir, Vec3:Normalize(Vec3:Subtract(myTankPos, targetPos))) < math.cos(math.rad(25))) then
			-- Now start  hunting the dude who ran us scared
			SwitchStrategy(STRATEGY_HUNT, currentStrategyTarget)
			return
		end
		
		if(Vec3:Dot(targetDir, myTankDir) > math.cos(math.rad(30))) then
			-- turn until we're in a position to bail				
			SetLeftTrackSpeed(-1.0)
			SetRightTrackSpeed(0.5)
		else
			SetLeftTrackSpeed(1.0)
			SetRightTrackSpeed(1.0)
		end
		
		AimAndFire(currentStrategyTarget)
	else
		SetLeftTrackSpeed(-1.0)
		SetRightTrackSpeed(1.0)
	end
end

function BrainTick(messages)
	if(run_cnt == 0) then
		Setup()
	end
	
	if(StuckCheck()) then
		return
	end
	
	ProcessMessages(messages)
	
	if(currentStrategy == STRATEGY_EXPLORE) then
		Explore()
	elseif(currentStrategy == STRATEGY_HUNT) then
		Hunt()
	elseif(currentStrategy == STRATEGY_RETREAT) then
		Retreat()
	end	
end

function SandboxRender()
end
