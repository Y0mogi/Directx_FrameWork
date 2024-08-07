#include "main.h"
#include "renderer.h"
#include "title.h"
#include "transform.h"
#include "game.h"
#include "input.h"
#include "manager.h"

#include "sprite.h"

#include "result.h"

//DirectWriteCustomFont* font{};
//FontData data;

void Title::Init()
{
	auto a = AddObjComp<Sprite>("Sprite", Layer::Layer_2, Tag::None,Sprite(XMFLOAT4{1,1,1,1},L"asset\\texture\\Brick.jpg"));
	a->GetComponent<Transform>()->position = { 200,200,0 };
	a->GetComponent<Transform>()->scale = { 100,100,0 };
	
	//if (font == nullptr) {
	//	font = new DirectWriteCustomFont(&data);
	//
	//	font->Init(Renderer::GetSwapChain());
	//	font->FontLoader();
	//
	//	font->GetFontFamilyName(font->fontCollection.Get(), L"ja-JP");
	//
	//	data.fontSize = 60;
	//	data.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	//	data.Color = D2D1::ColorF(D2D1::ColorF::White);
	//	data.font = L"ƒNƒ‰ƒtƒg–¾’©";
	//
	//	int b = font->GetFontNameNum();
	//	if (b > 0) {
	//		font->SetFont(data);
	//	}
	//}
	//

}

void Title::Uninit()
{
	Scene::Uninit();

	//if (font != nullptr) {
	//	delete font;
	//}
	//
	//font = nullptr;
	//NULLSEARCH(nullptr);
}

void Title::Update(const float& dt)
{
	Scene::Update(dt);

	if (Input::GetKeyTrigger('T')) Manager::SetScene<Game>();
}

void Title::Draw()
{
	Scene::Draw();
	static int num = 0;
	num--;

	//font->DrawString(std::to_string(num), XMFLOAT2(50, 50), D2D1_DRAW_TEXT_OPTIONS_NONE);
}
