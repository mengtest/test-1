--����Ľ�������
function Task_Accept_00000173()
	local player = GetPlayer();
	if player:GetLev() < 80 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(173) or task:HasCompletedTask(173) or task:HasSubmitedTask(173) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000173()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 80 then
		return false;
	end
	if task:HasAcceptedTask(173) or task:HasCompletedTask(173) or task:HasSubmitedTask(173) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000173()
	if GetPlayer():GetTaskMgr():HasCompletedTask(173) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000173(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(173) == npcId and Task_Accept_00000173 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 173
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "烈焰侍者";
	elseif task:GetTaskSubmitNpc(173) == npcId then
		if Task_Submit_00000173() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 173
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "烈焰侍者";
		elseif task:HasAcceptedTask(173) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 173
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "烈焰侍者";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000173_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "在这片海域附近，有一处奇异的火山叫做月儿岛，乃是昔年连山大师坐化之所，听说有不少奇珍遗留，你不如去探寻一下，说不定有所收获。不过月儿岛的守卫仍在，你要小心应付才对。";
	action.m_ActionMsg = "是吗，我这就去看看。";
	return action;
end

function Task_00000173_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠在月儿岛有何收获？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000173_step_table = {
		[1] = Task_00000173_step_01,
		[10] = Task_00000173_step_10,
		};

function Task_00000173_step(step)
	if Task_00000173_step_table[step] ~= nil then
		return Task_00000173_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000173_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000173() then
		return false;
	end
	if not task:AcceptTask(173) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000173_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(173) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--��������
function Task_00000173_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(173);
end