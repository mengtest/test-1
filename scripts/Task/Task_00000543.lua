--任务的接受条件
function Task_Accept_00000543()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(543) or task:HasCompletedTask(543) or task:HasSubmitedTask(543) then
		return false;
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000543()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(543) or task:HasCompletedTask(543) or task:HasSubmitedTask(543) then
		return false;
	end
	return true;
end


--任务完成条件
function Task_Submit_00000543()
	if GetPlayer():GetTaskMgr():HasCompletedTask(543) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000543(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(543) == npcId and Task_Accept_00000543 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 543
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_1364;
	elseif task:GetTaskSubmitNpc(543) == npcId then
		if Task_Submit_00000543() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 543
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_1365;
		elseif task:HasAcceptedTask(543) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 543
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_1366;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000543_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_1367..GetPlayerName(GetPlayer())..task_msg_1368;
	action.m_ActionMsg = task_msg_1369;
	return action;
end

function Task_00000543_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_1370;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000543_step_table = {
		[1] = Task_00000543_step_01,
		[10] = Task_00000543_step_10,
		};

function Task_00000543_step(step)
	if Task_00000543_step_table[step] ~= nil then
		return Task_00000543_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000543_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000543() then
		return false;
	end
	if not task:AcceptTask(543) then
		return false;
	end
	return true;
end



--提交任务
function Task_00000543_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(543) then
		return false;
	end


	return true;
end

--放弃任务
function Task_00000543_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(543);
end
