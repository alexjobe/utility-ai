#include "Editor/UTActionsPanel.h"
#include <Core/UTActionRegistry.h>
#include <imgui.h>

using namespace UAI;

void UTActionsPanel::Render()
{
	if (ImGui::BeginChild("ActionsList", ImVec2(0, 0), true))
	{
		const auto& Actions = UTActionRegistry::Instance().GetActions();
		for (const auto& [_, Action] : Actions)
		{
			RenderAction(Action);
		}
		ImGui::EndChild();
	}
}

void UTActionsPanel::RenderAction(const UTAction& Action) const
{
	if (ImGui::TreeNode(Action.GetKey().c_str()))
	{
		if (ImGui::TreeNode("Tags"))
		{
			for (auto& Tag : Action.Tags)
			{
				ImGui::BulletText("%s", Tag.c_str());
			}
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNode("Considerations"))
		{
			for (auto& [_, Consideration] : Action.GetConsiderations())
			{
				RenderConsideration(Consideration);
			}
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNode("Effects"))
		{
			for (auto& [_, Effect] : Action.GetEffects())
			{
				RenderEffect(Effect);
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

void UTActionsPanel::RenderConsideration(const UTConsideration& Consideration) const
{
	if (ImGui::TreeNode(Consideration.Key.c_str()))
	{
		RenderEvaluationData(Consideration.Data);
		ImGui::BulletText("RawScoreFn: %s", Consideration.GetRawScoreFnKey().c_str());
		ImGui::BulletText("ScoreCurveFn: %s", Consideration.GetScoreCurveFnKey().c_str());
		ImGui::TreePop();
	}
}

void UTActionsPanel::RenderEvaluationData(const UTEvaluationData& Data) const
{
	if (ImGui::TreeNode("Data"))
	{
		ImGui::BulletText("Target: %s", Data.Target.c_str());
		ImGui::BulletText("Raw: %f", Data.Raw);
		ImGui::BulletText("MinRaw: %f", Data.MinRaw);
		ImGui::BulletText("MaxRaw: %f", Data.MaxRaw);
		ImGui::BulletText("Weight: %f", Data.Weight);
		ImGui::BulletText("Priority: %d", Data.Priority);
		ImGui::TreePop();
	}
}

void UTActionsPanel::RenderEffect(const UTEffect& Effect) const
{
	if (ImGui::TreeNode(Effect.Key.c_str()))
	{
		RenderEvaluationData(Effect.Data);
		ImGui::BulletText("bIsConsideration: %s", Effect.bIsConsideration ? "True" : "False");
		ImGui::BulletText("RawScoreFn: %s", Effect.GetRawScoreFnKey().c_str());
		ImGui::BulletText("ScoreCurveFn: %s", Effect.GetScoreCurveFnKey().c_str());
		ImGui::BulletText("EffectFn: %s", Effect.GetEffectFnKey().c_str());
		ImGui::TreePop();
	}
}

