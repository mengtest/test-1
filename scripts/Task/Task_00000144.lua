--任务的接受条件
function Task_Accept_00000144()
	local player = GetPlayer();
	if player:GetLev() < 65 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(144) or task:HasCompletedTask(144) or task:HasSubmitedTask(144) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(143) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(143) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(143) then
			return false;
		end
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000144()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 65 then
		return false;
	end
	if task:HasAcceptedTask(144) or task:HasCompletedTask(144) or task:HasSubmitedTask(144) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(143) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(143) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(143) then
			return false;
		end
	end
	return true;
end


--任务完成条件
function Task_Submit_00000144()
	if GetPlayer():GetTaskMgr():HasCompletedTask(144) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000144(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(144) == npcId and Task_Accept_00000144 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 144
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_1526;
	elseif task:GetTaskSubmitNpc(144) == npcId then
		if Task_Submit_00000144() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 144
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_1527;
		elseif task:HasAcceptedTask(144) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 144
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_1528;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000144_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_1529;
	action.m_ActionMsg = task_msg_1530;
	return action;
end

function Task_00000144_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer())..task_msg_1531;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000144_step_table = {
		[1] = Task_00000144_step_01,
		[10] = Task_00000144_step_10,
		};

function Task_00000144_step(step)
	if Task_00000144_step_table[step] ~= nil then
		return Task_00000144_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000144_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000144() then
		return false;
	end
	if not task:AcceptTask(144) then
		return false;
	end
	return true;
end



--提交任务
function Task_00000144_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(1267,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 1013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(144) then
		return false;
	end

	if IsEquipTypeId(1267) then
		for k = 1, 1 do
			package:AddEquip(1267, 1);
		end
	else 
		package:AddItem(1267,1,1);
	end

	player:AddExp(100000);
	return true;
end

--放弃任务
function Task_00000144_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(144);
end
