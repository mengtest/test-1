--����Ľ�������
function Task_Accept_00000155()
	local player = GetPlayer();
	if player:GetLev() < 70 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(155) or task:HasCompletedTask(155) or task:HasSubmitedTask(155) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000155()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 70 then
		return false;
	end
	if task:HasAcceptedTask(155) or task:HasCompletedTask(155) or task:HasSubmitedTask(155) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000155()
	if GetPlayer():GetTaskMgr():HasCompletedTask(155) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000155(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(155) == npcId and Task_Accept_00000155 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 155
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "紫云之乱";
	elseif task:GetTaskSubmitNpc(155) == npcId then
		if Task_Submit_00000155() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 155
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "紫云之乱";
		elseif task:HasAcceptedTask(155) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 155
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "紫云之乱";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000155_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "紫云宫原是连山大师别府，天一金母的旧居，这紫云三女前世乃天一金母的侍女，此番转世重来，却误入歧途，勾结邪魔外道。这回令你去东海，就是要将这些邪派人士铲除。";
	action.m_ActionMsg = "嗯，我这就去。";
	return action;
end

function Task_00000155_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "紫云宫原是仙人府邸，但是现在却被这群妖人占据。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000155_step_table = {
		[1] = Task_00000155_step_01,
		[10] = Task_00000155_step_10,
		};

function Task_00000155_step(step)
	if Task_00000155_step_table[step] ~= nil then
		return Task_00000155_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000155_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000155() then
		return false;
	end
	if not task:AcceptTask(155) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000155_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(155) then
		return false;
	end


	player:AddExp(90000);
	return true;
end

--��������
function Task_00000155_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(155);
end