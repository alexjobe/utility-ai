#include "Editor/UTPanelHelpers.h"
#include <imgui.h>

void UTEditor::RenderConsideration(const UTConsideration& Consideration)
{
	if (ImGui::TreeNode(Consideration.Key.c_str()))
	{
		RenderEvaluationData(Consideration.Data);
		ImGui::BulletText("RawScoreFn: %s", Consideration.GetRawScoreFnKey().c_str());
		ImGui::BulletText("ScoreCurveFn: %s", Consideration.GetScoreCurveFnKey().c_str());
		ImGui::TreePop();
	}
}

void UTEditor::RenderEffect(const UTEffect& Effect)
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

void UTEditor::RenderEvaluationData(const UTEvaluationData& Data)
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

void UTEditor::RenderAction(const UTAction& Action)
{
	if (ImGui::TreeNode(Action.GetName().c_str()))
	{
		if (ImGui::TreeNode("OwnedTags"))
		{
			for (auto& Tag : Action.OwnedTags)
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

		ImGui::Separator();
		ImGui::BulletText("PreconditionFn: %s", Action.Scorer.GetPreconditionFnKey().c_str());

		ImGui::TreePop();
	}
}

void UTEditor::RenderGoal(const UTGoal& Goal)
{
	if (ImGui::TreeNode(Goal.GetName().c_str()))
	{
		if (ImGui::TreeNode("OwnedTags"))
		{
			for (auto& Tag : Goal.OwnedTags)
			{
				ImGui::BulletText("%s", Tag.c_str());
			}
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNode("RequiredTags"))
		{
			for (auto& Tag : Goal.RequiredTags)
			{
				ImGui::BulletText("%s", Tag.c_str());
			}
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNode("Considerations"))
		{
			for (auto& [_, Consideration] : Goal.GetConsiderations())
			{
				RenderConsideration(Consideration);
			}
			ImGui::TreePop();
		}

		ImGui::Separator();
		ImGui::BulletText("PreconditionFn: %s", Goal.Scorer.GetPreconditionFnKey().c_str());

		ImGui::TreePop();
	}
}

void UTEditor::RenderTrait(const UTTrait& Trait)
{
	if (ImGui::TreeNode(Trait.GetName().c_str()))
	{
		if (ImGui::TreeNode("OwnedTags"))
		{
			for (auto& Tag : Trait.OwnedTags)
			{
				ImGui::BulletText("%s", Tag.c_str());
			}
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNode("RequiredTags"))
		{
			for (auto& Tag : Trait.RequiredTags)
			{
				ImGui::BulletText("%s", Tag.c_str());
			}
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNode("Considerations"))
		{
			for (auto& [_, Consideration] : Trait.GetConsiderations())
			{
				RenderConsideration(Consideration);
			}
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNode("Effects"))
		{
			for (auto& [_, Effect] : Trait.GetEffects())
			{
				RenderEffect(Effect);
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}
