--����Ľ�������
function Task_Accept_00000531()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(531) or task:HasCompletedTask(531) or task:HasSubmitedTask(531) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000531()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(531) or task:HasCompletedTask(531) or task:HasSubmitedTask(531) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000531()
	if GetPlayer():GetTaskMgr():HasCompletedTask(531) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000531(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(531) == npcId and Task_Accept_00000531 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 531
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "水贼头目";
	elseif task:GetTaskSubmitNpc(531) == npcId then
		if Task_Submit_00000531() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 531
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "水贼头目";
		elseif task:HasAcceptedTask(531) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 531
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "水贼头目";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000531_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "成都前段时间治安真得很成问题，有一股水贼在附近为恶，他们的头目是个叫李刚的家伙，残暴异常，"..GetPlayerName(GetPlayer()).."速速去将这个恶人铲除。";
	action.m_ActionMsg = "为民除害是我辈本色，弟子去去就回。";
	return action;
end

function Task_00000531_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哎，我们修道之人虽说不问尘事，可是兵祸连连，我也不忍撒手不顾。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000531_step_table = {
		[1] = Task_00000531_step_01,
		[10] = Task_00000531_step_10,
		};

function Task_00000531_step(step)
	if Task_00000531_step_table[step] ~= nil then
		return Task_00000531_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000531_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000531() then
		return false;
	end
	if not task:AcceptTask(531) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000531_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(531) then
		return false;
	end


	return true;
end

--��������
function Task_00000531_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(531);
end