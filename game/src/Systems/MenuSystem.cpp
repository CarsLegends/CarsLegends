#include "MenuSystem.hpp"

#include <utility>

namespace Systems
{
	MenuSystem::MenuSystem() = default;

	void MenuSystem::Initialize(std::shared_ptr<Coordinator> coordinator, Window window)
	{
		this->mCoordinator = std::move(coordinator);
		this->mNkContext = window.getNkContext();
	}

	void MenuSystem::Update(float deltaTime)
	{
		enum {EASY, HARD};
		static int op = EASY;
		static float value = 0.6f;
		static int i =  20;

		if (nk_begin(mNkContext, "Show", nk_rect(50, 50, 220, 220),
				NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
				// fixed widget pixel width
				nk_layout_row_static(mNkContext, 30, 80, 1);
				if (nk_button_label(mNkContext, "button")) {
						// event handling
				}
				// fixed widget window ratio width
				nk_layout_row_dynamic(mNkContext, 30, 2);
				if (nk_option_label(mNkContext, "easy", op == EASY)) op = EASY;
				if (nk_option_label(mNkContext, "hard", op == HARD)) op = HARD;
				// custom widget pixel width
				nk_layout_row_begin(mNkContext, NK_STATIC, 30, 2);
				{
						nk_layout_row_push(mNkContext, 50);
						nk_label(mNkContext, "Volume:", NK_TEXT_LEFT);
						nk_layout_row_push(mNkContext, 110);
						nk_slider_float(mNkContext, 0, &value, 1.0f, 0.1f);
				}
				nk_layout_row_end(mNkContext);
		}
		nk_end(mNkContext);

		nk_clear(mNkContext);
	}
}
