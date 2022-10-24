#include "ViewProjection.h"

void ViewProjection::Initialize() {
	eye = {0 , 0 , -50};	//視点座標
	target = {0 , 0 , 0};	//注視点座標
	up = {0 , 1 , 0};		//上方向ベクトル
	UpdateView();
}

void ViewProjection::UpdateView() {

	matView = MathFunc::Utility::CreatMatView(eye , target , up);

}