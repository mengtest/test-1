--����Ľ�������
function Task_Accept_00000181()
	local player = GetPlayer();
	if player:GetLev() < 85 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(181) or task:HasCompletedTask(181) or task:HasSubmitedTask(181) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(180) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(180) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(180) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000181()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 85 then
		return false;
	end
	if task:HasAcceptedTask(181) or task:HasCompletedTask(181) or task:HasSubmitedTask(181) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(180) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(180) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(180) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000181()
	if GetPlayer():GetTaskMgr():HasCompletedTask(181) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000181(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(181) == npcId and Task_Accept_00000181 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 181
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "灵灵子";
	elseif task:GetTaskSubmitNpc(181) == npcId then
		if Task_Submit_00000181() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 181
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "灵灵子";
		elseif task:HasAcceptedTask(181) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 181
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "灵灵子";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000181_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我本是昆仑派的剑侠，与同门有些干戈，后来介绍武当长老的邀请这才入了武当，如今执掌武当。这些内部事务还得请教师兄灵灵子。";
	action.m_ActionMsg = "是吗，我这就去看看。";
	return action;
end

function Task_00000181_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠真是修为深厚，天赋异禀啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000181_step_table = {
		[1] = Task_00000181_step_01,
		[10] = Task_00000181_step_10,
		};

function Task_00000181_step(step)
	if Task_00000181_step_table[step] ~= nil then
		return Task_00000181_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000181_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000181() then
		return false;
	end
	if not task:AcceptTask(181) then
		return false;
	end
	task:AddTaskStep(181);
	return true;
end



--�ύ����
function Task_00000181_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(181) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--��������
function Task_00000181_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(181);
end