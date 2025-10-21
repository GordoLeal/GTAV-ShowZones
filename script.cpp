#pragma once
#include "script.h"
#include "keyboard.h"
#include <cmath>
#include <iostream>
#include <string>
#include <debugapi.h>

// REMINDER: SET THIS BACK TO TRUE AFTER IS DONE, OTHERWISE YOU GONNA HAVE BAD STUFF HAPPENING!!!
static void SET_BACKFACECULLING(BOOL p0) { invoke<Void>(0x23BA6B0C2AD7B0D3, p0); } // 0x23BA6B0C2AD7B0D3 0xC44C2F44
static int GET_WAYPOINT_BLIP_ENUM_ID() { return invoke<int>(0x186E5D252FA50E7D); } // 0x186E5D252FA50E7D 0xB9827942

struct GameColor
{
	int r = 0;
	int g = 0;
	int b = 0;
	int a = 255;
	GameColor(int r = 0, int g = 0, int b = 0, int a = 255) : r(r), g(g), b(b), a(a) {}
};

struct Vec3
{
	float x, y, z;
	Vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
	Vector3 ToScripthookVec3() {
		Vector3 out;
		out.x = x;
		out.y = y;
		out.z = z;
		return out;
	}
	Vec3(Vector3 a) : x(a.x), y(a.y), z(a.z) {}
};

static void RemoveUselessBlip(Blip blip) {
	if (UI::DOES_BLIP_EXIST(blip)) {
		UI::REMOVE_BLIP(&blip);
	}
}

static bool IsScriptActive(char* script)
{
	return SCRIPT::_GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT(GAMEPLAY::GET_HASH_KEY(script)) > 0;
}

static void DrawFace(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4, GameColor color)
{
	GRAPHICS::DRAW_POLY(p3.x, p3.y, p3.z, p2.x, p2.y, p2.z, p1.x, p1.y, p1.z, color.r, color.g, color.b, color.a);
	GRAPHICS::DRAW_POLY(p1.x, p1.y, p1.z, p4.x, p4.y, p4.z, p3.x, p3.y, p3.z, color.r, color.g, color.b, color.a);
}

static void DrawAngledZone(Vec3 a, Vec3 b, float dis, bool bothSides = true)
{
	// A -- B
	// |    |
	// D __ C
	// Distancia da linha entre A e B
	Vec3 v;
	v.x = b.x - a.x;
	v.y = b.y - a.y;
	// inverter a posição e sinal de um dos valores
	float s;
	s = v.x;
	v.x = v.y * -1;
	v.y = s;
	// Comprimento da direção unitaria.
	float du = std::sqrt((v.x * v.x) + (v.y * v.y));
	// Mover a direção unitaria na direção que quero encontrar os pontos
	float dux, duy;
	dux = v.x / du;
	duy = v.y / du;
	// deslocando na direção que tenho pela distancia para encontrar os pontos.
	float offsetx, offsety;
	offsetx = (dis / 2) * dux;
	offsety = (dis / 2) * duy;
	// Agora com o offset, aplicar em cima de um dos pontos da linha
	// e encontra os outros lados.
	Vec3 pA;
	pA.x = a.x + offsetx;
	pA.y = a.y + offsety;
	pA.z = a.z;
	Vec3 pB;
	pB.x = a.x - offsetx;
	pB.y = a.y - offsety;
	pB.z = a.z;
	Vec3 pC;
	pC.x = a.x - offsetx;
	pC.y = a.y - offsety;
	pC.z = b.z;
	Vec3 pD;
	pD.x = a.x + offsetx;
	pD.y = a.y + offsety;
	pD.z = b.z;

	Vec3 pA2;
	pA2.x = b.x + offsetx;
	pA2.y = b.y + offsety;
	pA2.z = a.z;
	Vec3 pB2;
	pB2.x = b.x - offsetx;
	pB2.y = b.y - offsety;
	pB2.z = a.z;
	Vec3 pC2;
	pC2.x = b.x - offsetx;
	pC2.y = b.y - offsety;
	pC2.z = b.z;
	Vec3 pD2;
	pD2.x = b.x + offsetx;
	pD2.y = b.y + offsety;
	pD2.z = b.z;

	GRAPHICS::DRAW_LINE(a.x, a.y, a.z, b.x, b.y, b.z, 2, 120, 120, 255);

	// AB
	GRAPHICS::DRAW_LINE(pA.x, pA.y, pA.z, pB.x, pB.y, pB.z, 0, 255, 0, 255);
	// BC
	GRAPHICS::DRAW_LINE(pB.x, pB.y, pB.z, pC.x, pC.y, pC.z, 255, 120, 100, 255);
	// CD
	GRAPHICS::DRAW_LINE(pC.x, pC.y, pC.z, pD.x, pD.y, pD.z, 50, 160, 255, 255);
	// DA
	GRAPHICS::DRAW_LINE(pD.x, pD.y, pD.z, pA.x, pA.y, pA.z, 92, 187, 8, 255);

	// AB 2
	GRAPHICS::DRAW_LINE(pA2.x, pA2.y, pA2.z, pB2.x, pB2.y, pB2.z, 0, 255, 0, 255);
	// BC 2
	GRAPHICS::DRAW_LINE(pB2.x, pB2.y, pB2.z, pC2.x, pC2.y, pC2.z, 255, 120, 100, 255);
	// CD 2
	GRAPHICS::DRAW_LINE(pC2.x, pC2.y, pC2.z, pD2.x, pD2.y, pD2.z, 50, 160, 255, 255);
	// DA 2
	GRAPHICS::DRAW_LINE(pD2.x, pD2.y, pD2.z, pA2.x, pA2.y, pA2.z, 92, 187, 8, 255);

	GRAPHICS::DRAW_LINE(pA.x, pA.y, pA.z, pA2.x, pA2.y, pA2.z, 252, 123, 0, 255);
	GRAPHICS::DRAW_LINE(pB.x, pB.y, pB.z, pB2.x, pB2.y, pB2.z, 252, 123, 0, 255);
	GRAPHICS::DRAW_LINE(pC.x, pC.y, pC.z, pC2.x, pC2.y, pC2.z, 252, 123, 0, 255);
	GRAPHICS::DRAW_LINE(pD.x, pD.y, pD.z, pD2.x, pD2.y, pD2.z, 252, 123, 0, 255);

	if (bothSides)
		SET_BACKFACECULLING(false);
	//front A side
	GameColor frontColor;
	frontColor.r = 255;
	frontColor.g = 0;
	frontColor.b = 0;
	frontColor.a = 25;
	DrawFace(pA, pB, pC, pD, frontColor);

	//front B side
	GameColor frontBColor;
	frontBColor.r = 0;
	frontBColor.g = 0;
	frontBColor.b = 255;
	frontBColor.a = 25;
	DrawFace(pB2, pA2, pD2, pC2, frontBColor);

	//Left
	GameColor leftColor;
	leftColor.r = 0;
	leftColor.g = 255;
	leftColor.b = 0;
	leftColor.a = 25;
	DrawFace(pB, pB2, pC2, pC, leftColor);

	//Right
	GameColor rightColor;
	rightColor.r = 0;
	rightColor.g = 255;
	rightColor.b = 220;
	rightColor.a = 25;
	DrawFace(pA2, pA, pD, pD2, rightColor);

	//Top
	GameColor topColor;
	topColor.r = 245;
	topColor.g = 0;
	topColor.b = 255;
	topColor.a = 25;
	DrawFace(pA2, pB2, pB, pA, topColor);

	//Bottom
	GameColor bottomColor;
	bottomColor.r = 125;
	bottomColor.g = 255;
	bottomColor.b = 125;
	bottomColor.a = 25;
	DrawFace(pC2, pD2, pD, pC, bottomColor);

	//if (bothSides) {
	//	//front A side
	//	GameColor frontColor;
	//	frontColor.r = 255;
	//	frontColor.g = 0;
	//	frontColor.b = 0;
	//	frontColor.a = 25;
	//	DrawFace(pB, pA, pD, pC, frontColor);

	//	//front B side
	//	GameColor frontBColor;
	//	frontBColor.r = 0;
	//	frontBColor.g = 0;
	//	frontBColor.b = 255;
	//	frontBColor.a = 25;
	//	DrawFace(pA2, pB2, pC2, pD2, frontBColor);

	//	//Left side
	//	GameColor leftColor;
	//	leftColor.r = 0;
	//	leftColor.g = 255;
	//	leftColor.b = 0;
	//	leftColor.a = 25;
	//	DrawFace(pB2, pB, pC, pC2, leftColor);

	//	//Right
	//	GameColor rightColor;
	//	rightColor.r = 0;
	//	rightColor.g = 255;
	//	rightColor.b = 220;
	//	rightColor.a = 25;
	//	DrawFace(pA, pA2, pD2, pD, rightColor);

	//	//Top
	//	GameColor topColor;
	//	topColor.r = 245;
	//	topColor.g = 0;
	//	topColor.b = 255;
	//	topColor.a = 25;
	//	DrawFace(pB2, pA2, pA, pB, topColor);

	//	//Bottom
	//	GameColor bottomColor;
	//	bottomColor.r = 125;
	//	bottomColor.g = 125;
	//	bottomColor.b = 125;
	//	bottomColor.a = 25;
	//	DrawFace(pD2, pC2, pC, pD, bottomColor);
	//}
	if (bothSides)
		SET_BACKFACECULLING(true);
	////test
	//Vector3 pPos = ENTITY::GET_ENTITY_COORDS( PLAYER::PLAYER_PED_ID(),false);
	//GRAPHICS::DRAW_LINE(pA.x, pA.y, pA.z, pPos.x, pPos.y, pPos.z, 255, 0, 0, 255);
	//GRAPHICS::DRAW_LINE(pB.x, pB.y, pB.z, pPos.x, pPos.y, pPos.z, 0, 255, 0, 255);
	//GRAPHICS::DRAW_LINE(pC.x, pC.y, pC.z, pPos.x, pPos.y, pPos.z, 0, 0, 255, 255);
	//GRAPHICS::DRAW_LINE(pA2.x, pA2.y, pA2.z, pPos.x, pPos.y, pPos.z, 255, 0, 0, 255);
	//GRAPHICS::DRAW_LINE(pB2.x, pB2.y, pB2.z, pPos.x, pPos.y, pPos.z, 0, 255, 0, 255);
	//GRAPHICS::DRAW_LINE(pC2.x, pC2.y, pC2.z, pPos.x, pPos.y, pPos.z, 0, 0, 255, 255);
}

static void DrawLine(Vec3 start, Vec3 end, GameColor color) {
	GRAPHICS::DRAW_LINE(start.x, start.y, start.z, end.x, end.y, end.z, color.r, color.g, color.b, color.a);
}

//Custom version of DRAW_BOX because for some reason the original call conflicts with other draw_calls and i don't want to deal with it.
//This box is gonna be rotated in the North direction, like the original call.
static void DrawBox(Vec3 pos1, Vec3 pos2, GameColor color, bool drawBothSides)
{
	Vec3 sA = pos1;
	Vec3 sB = Vec3(pos2.x, pos1.y, pos1.z);
	Vec3 sC = Vec3(pos2.x, pos1.y, pos2.z);
	Vec3 sD = Vec3(pos1.x, pos1.y, pos2.z);

	Vec3 sE = Vec3(pos1.x, pos2.y, pos1.z);
	Vec3 sF = Vec3(pos2.x, pos2.y, pos1.z);
	Vec3 sG = pos2;
	Vec3 sH = Vec3(pos1.x, pos2.y, pos2.z);

	if (drawBothSides)
		SET_BACKFACECULLING(false);
	DrawFace(sA, sB, sC, sD, color); // FRONT
	DrawFace(sB, sF, sG, sC, color); // RIGHT
	DrawFace(sE, sA, sD, sH, color); // LEFT
	DrawFace(sF, sE, sH, sG, color); // BACK
	DrawFace(sD, sC, sG, sH, color); // TOP
	DrawFace(sB, sA, sE, sF, color); // DOWN
	if (drawBothSides)
		SET_BACKFACECULLING(true);
}

static void DrawBasicText(char* text, float x, float y, float scaleY, int font = 0, int align = 1)
{
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0, scaleY);
	UI::SET_TEXT_COLOUR(255, 255, 255, 255);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	//	FONT_CENTRE = 0
	//	FONT_LEFT,= 1
	//	FONT_RIGHT = 2
	//	FONT_JUSTIFY= 3
	UI::SET_TEXT_JUSTIFICATION(align);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY((char*)"STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	UI::_DRAW_TEXT(x, y);
}

static void DrawBasicTextWithColor(char* text, float x, float y, float scaleY, GameColor color, int font = 0, int align = 1)
{
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0, scaleY);
	UI::SET_TEXT_COLOUR(color.r, color.g, color.b, color.a);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	//	FONT_CENTRE = 0
	//	FONT_LEFT,= 1
	//	FONT_RIGHT = 2
	//	FONT_JUSTIFY= 3
	UI::SET_TEXT_JUSTIFICATION(align);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY((char*)"STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	UI::_DRAW_TEXT(x, y);
}


static void QuickDrawAngledZone(float x1, float y1, float z1, float x2, float y2, float z2, float dis)
{
	DrawAngledZone(Vec3(x1, y1, z1), Vec3(x2, y2, z2), dis, true);
}

static void DrawScuffedSquareCheck(Vec3 pos, Vec3 size, bool drawBothSides = true) {
	GameColor pink = { 255,0,255 };
	//Central lines
	Vec3 lAS = Vec3(pos.x - size.x, pos.y, pos.z);
	Vec3 lAE = Vec3(pos.x + size.x, pos.y, pos.z);
	GameColor lAC = { 255,0,0 };
	DrawLine(lAS, lAE, lAC);
	Vec3 lBS = Vec3(pos.x, pos.y - size.y, pos.z);
	Vec3 lBE = Vec3(pos.x, pos.y + size.y, pos.z);
	GameColor lBC = { 0,255,0 };
	DrawLine(lBS, lBE, lBC);
	Vec3 lCS = Vec3(pos.x, pos.y, pos.z - size.z);
	Vec3 lCE = Vec3(pos.x, pos.y, pos.z + size.z);
	GameColor lCC = { 0,0,255 };
	DrawLine(lCS, lCE, lCC);

	//Side lines
	Vec3 lS1S = Vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z);
	Vec3 lS1E = Vec3(pos.x + size.x, pos.y + size.y, pos.z - size.z);
	DrawLine(lS1S, lS1E, pink);
	Vec3 lS2S = Vec3(pos.x - size.x, pos.y + size.y, pos.z + size.z);
	Vec3 lS2E = Vec3(pos.x - size.x, pos.y + size.y, pos.z - size.z);
	DrawLine(lS2S, lS2E, pink);
	Vec3 lS3S = Vec3(pos.x - size.x, pos.y - size.y, pos.z + size.z);
	Vec3 lS3E = Vec3(pos.x - size.x, pos.y - size.y, pos.z - size.z);
	DrawLine(lS3S, lS3E, pink);
	Vec3 lS4S = Vec3(pos.x + size.x, pos.y - size.y, pos.z + size.z);
	Vec3 lS4E = Vec3(pos.x + size.x, pos.y - size.y, pos.z - size.z);
	DrawLine(lS4S, lS4E, pink);

	//Top
	Vec3 lS5S = Vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z);
	Vec3 lS5E = Vec3(pos.x + size.x, pos.y - size.y, pos.z + size.z);
	DrawLine(lS5S, lS5E, pink);
	Vec3 lS6S = Vec3(pos.x - size.x, pos.y + size.y, pos.z + size.z);
	Vec3 lS6E = Vec3(pos.x - size.x, pos.y - size.y, pos.z + size.z);
	DrawLine(lS6S, lS6E, pink);
	Vec3 lS7E = Vec3(pos.x - size.x, pos.y + size.y, pos.z + size.z);
	Vec3 lS7S = Vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z);
	DrawLine(lS7S, lS7E, pink);
	Vec3 lS8S = Vec3(pos.x - size.x, pos.y - size.y, pos.z + size.z);
	Vec3 lS8E = Vec3(pos.x + size.x, pos.y - size.y, pos.z + size.z);
	DrawLine(lS8S, lS8E, pink);

	//Bottom
	Vec3 lS9S = Vec3(pos.x + size.x, pos.y + size.y, pos.z - size.z);
	Vec3 lS9E = Vec3(pos.x + size.x, pos.y - size.y, pos.z - size.z);
	DrawLine(lS9S, lS9E, pink);
	Vec3 lS10E = Vec3(pos.x - size.x, pos.y - size.y, pos.z - size.z);
	Vec3 lS10S = Vec3(pos.x - size.x, pos.y + size.y, pos.z - size.z);
	DrawLine(lS10S, lS10E, pink);
	Vec3 lS11S = Vec3(pos.x + size.x, pos.y + size.y, pos.z - size.z);
	Vec3 lS11E = Vec3(pos.x - size.x, pos.y + size.y, pos.z - size.z);
	DrawLine(lS11S, lS11E, pink);
	Vec3 lS12S = Vec3(pos.x - size.x, pos.y - size.y, pos.z - size.z);
	Vec3 lS12E = Vec3(pos.x + size.x, pos.y - size.y, pos.z - size.z);
	DrawLine(lS12S, lS12E, pink);

	if (drawBothSides)
		SET_BACKFACECULLING(false);

	//TOP	
	DrawFace(
		Vec3(pos.x - size.x, pos.y + size.y, pos.z + size.z),
		Vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z),
		Vec3(pos.x + size.x, pos.y - size.y, pos.z + size.z),
		Vec3(pos.x - size.x, pos.y - size.y, pos.z + size.z),
		GameColor(0, 255, 0, 30)
	);

	//bottom	
	DrawFace(
		Vec3(pos.x + size.x, pos.y + size.y, pos.z - size.z),
		Vec3(pos.x - size.x, pos.y + size.y, pos.z - size.z),
		Vec3(pos.x - size.x, pos.y - size.y, pos.z - size.z),
		Vec3(pos.x + size.x, pos.y - size.y, pos.z - size.z),
		GameColor(0, 255, 0, 30)
	);
	//Back	
	DrawFace(
		Vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z),
		Vec3(pos.x - size.x, pos.y + size.y, pos.z + size.z),
		Vec3(pos.x - size.x, pos.y + size.y, pos.z - size.z),
		Vec3(pos.x + size.x, pos.y + size.y, pos.z - size.z),
		GameColor(255, 0, 0, 30)
	);
	//Front
	DrawFace(
		Vec3(pos.x - size.x, pos.y - size.y, pos.z + size.z),
		Vec3(pos.x + size.x, pos.y - size.y, pos.z + size.z),
		Vec3(pos.x + size.x, pos.y - size.y, pos.z - size.z),
		Vec3(pos.x - size.x, pos.y - size.y, pos.z - size.z),
		GameColor(255, 0, 0, 30)
	);

	//Right
	DrawFace(
		Vec3(pos.x + size.x, pos.y - size.y, pos.z + size.z),
		Vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z),
		Vec3(pos.x + size.x, pos.y + size.y, pos.z - size.z),
		Vec3(pos.x + size.x, pos.y - size.y, pos.z - size.z),
		GameColor(0, 0, 255, 30)
	);

	//Left
	DrawFace(
		Vec3(pos.x - size.x, pos.y + size.y, pos.z + size.z),
		Vec3(pos.x - size.x, pos.y - size.y, pos.z + size.z),
		Vec3(pos.x - size.x, pos.y - size.y, pos.z - size.z),
		Vec3(pos.x - size.x, pos.y + size.y, pos.z - size.z),
		GameColor(0, 255, 220, 30)
	);

	if (drawBothSides)
		SET_BACKFACECULLING(true);
	//if (drawBothSides)
	//{
	//	//TOP	
	//	DrawFace(
	//		Vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z),
	//		Vec3(pos.x - size.x, pos.y + size.y, pos.z + size.z),
	//		Vec3(pos.x - size.x, pos.y - size.y, pos.z + size.z),
	//		Vec3(pos.x + size.x, pos.y - size.y, pos.z + size.z),
	//		GameColor(0, 255, 0, 30)
	//	);
	//	//Back	
	//	DrawFace(
	//		Vec3(pos.x - size.x, pos.y + size.y, pos.z + size.z),
	//		Vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z),
	//		Vec3(pos.x + size.x, pos.y + size.y, pos.z - size.z),
	//		Vec3(pos.x - size.x, pos.y + size.y, pos.z - size.z),
	//		GameColor(255, 0, 0, 30)
	//	);
	//	//Front
	//	DrawFace(
	//		Vec3(pos.x + size.x, pos.y - size.y, pos.z + size.z),
	//		Vec3(pos.x - size.x, pos.y - size.y, pos.z + size.z),
	//		Vec3(pos.x - size.x, pos.y - size.y, pos.z - size.z),
	//		Vec3(pos.x + size.x, pos.y - size.y, pos.z - size.z),
	//		GameColor(255, 0, 0, 30)
	//	);

	//	//Right
	//	DrawFace(
	//		Vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z),
	//		Vec3(pos.x + size.x, pos.y - size.y, pos.z + size.z),
	//		Vec3(pos.x + size.x, pos.y - size.y, pos.z - size.z),
	//		Vec3(pos.x + size.x, pos.y + size.y, pos.z - size.z),
	//		GameColor(0, 0, 255, 30)
	//	);

	//	//Left
	//	DrawFace(
	//		Vec3(pos.x - size.x, pos.y - size.y, pos.z + size.z),
	//		Vec3(pos.x - size.x, pos.y + size.y, pos.z + size.z),
	//		Vec3(pos.x - size.x, pos.y + size.y, pos.z - size.z),
	//		Vec3(pos.x - size.x, pos.y - size.y, pos.z - size.z),
	//		GameColor(0, 255, 220, 30)
	//	);
	//}

}
static void DrawScuffedSquareCheck(Vec3 pos, float fixedSize, bool drawBothSides = true)
{
	DrawScuffedSquareCheck(pos, Vec3(fixedSize, fixedSize, fixedSize), drawBothSides);
}

/*CONST_FLOAT LOCATE_SIZE_ANY_MEANS		6.0
	CONST_FLOAT LOCATE_SIZE_ON_FOOT_ONLY	4.0
	CONST_FLOAT	LOCATE_SIZE_HEIGHT			2.0
	CONST_FLOAT LOCATE_SIZE_MISSION_TRIGGER	2.75*/

	// VECTOR g_vAnyMeansLocate	= << LOCATE_SIZE_ANY_MEANS (6.0), LOCATE_SIZE_ANY_MEANS (6.0), LOCATE_SIZE_HEIGHT (2.0) >>
	// VECTOR g_vOnFootLocate = << LOCATE_SIZE_ON_FOOT_ONLY, LOCATE_SIZE_ON_FOOT_ONLY, LOCATE_SIZE_HEIGHT >>

static void DoPedCheckUpdate(Ped& ped, char* modelName) {
	if (ped == 0)
	{
		const int ARR_SIZE = 255;
		Vehicle pedInWorld[ARR_SIZE];
		int pedInWorldCount = worldGetAllPeds(pedInWorld, ARR_SIZE);
		int loopped = 0;
		bool foundped = false;
		do {
			Ped pedTest = pedInWorld[loopped];
			if (PED::IS_PED_MODEL(pedTest, GAMEPLAY::GET_HASH_KEY(modelName)) == TRUE)
			{
				foundped = true;
				ped = pedTest;
			}
			loopped++;
		} while (!foundped && loopped < pedInWorldCount);

	}
	else {
		if (PED::IS_PED_MODEL(ped, GAMEPLAY::GET_HASH_KEY(modelName)) == FALSE) {
			ped = 0;
		}
	}
}

//This will get the first vehicle with the modelName and is a mission vehicle.
static void DoVehCheckUpdate(Vehicle& veh, char* modelName)
{
	if (veh == 0) {
		const int ARR_SIZE = 255;
		Vehicle vehInWorld[ARR_SIZE];
		int vehInWorldCount = worldGetAllVehicles(vehInWorld, ARR_SIZE);
		int loopveh = 0;
		bool foundveh = false;
		do {
			Vehicle vehTrailerTest = vehInWorld[loopveh];
			if (VEHICLE::IS_VEHICLE_MODEL(vehTrailerTest, GAMEPLAY::GET_HASH_KEY(modelName)) == TRUE) {
				if (ENTITY::IS_ENTITY_A_MISSION_ENTITY(vehTrailerTest)) {
					foundveh = true;
					veh = vehTrailerTest;
				}
			}
			loopveh++;
		} while (!foundveh && loopveh < vehInWorldCount);
	}
	else
	{
		if (VEHICLE::IS_VEHICLE_MODEL(veh, GAMEPLAY::GET_HASH_KEY(modelName)) == FALSE) {
			veh = 0;
		}
	}
}


// =-=-=-=-=-=-=-=--=-==-=-- SCRIPTS LOGIC =-=-=-=-=--=--=-=---=--=--=--

static void Script_Prologue1()
{
	// Player learns to walk trigger.
	//QuickDrawAngledZone(5305.574707f, -5205.736816f, 86.518654f, 5316.579590f, -5205.753906f, 82.518654f, 5.0f);
	DrawScuffedSquareCheck(Vec3(5309.496094f, -5207.671875f, 84.018631f), Vec3(1.0f, 1.5f, 1.5f), true);

	// Get cash
	DrawAngledZone(Vec3(5307.583496f, -5184.533203f, 82.018890f), Vec3(5307.440918f, -5191.546875f, 86.567856f), 3.0f, true);
	DrawAngledZone(Vec3(5307.583496f, -5184.533203f, 82.018890f), Vec3(5307.440918f, -5191.546875f, 86.567856f), 4.5f, true);

	// Leave vault
	DrawAngledZone(Vec3(5298.023926f, -5189.134766f, 82.518318f), Vec3(5302.975586f, -5189.083008f, 85.518600f), 5.5f, true);

	// slowdown before garage door
	DrawAngledZone(Vec3(5320.420410f, -5182.820800f, 82.518669f), Vec3(5315.878418f, -5182.866699f, 85.518669f), 3.5f, true);

	// Trevor mask - Just for testing the mod
	//DrawScuffedSquareCheck(Vec3(5332.167480f, -5185.253906f, 83.80938f), Vec3(0.5f,0.5f, 2.0f),true);
	//DrawScuffedSquareCheck(Vec3(5332.145020f, -5191.842773f, 83.773270f), Vec3(0.5f, 0.5f, 2.0f),true);
	//DrawScuffedSquareCheck(Vec3(5326.206055f, -5185.222168f, 83.793671f), Vec3(0.5f, 0.5f, 2.0f),true);
	//DrawScuffedSquareCheck(Vec3(5326.216797f, -5191.865234f, 83.776054f), Vec3(0.5f, 0.5f, 2.0f),true);
	//DrawScuffedSquareCheck(Vec3(5332.157227f, -5178.680176f, 83.872810f), Vec3(0.5f, 0.5f, 2.0f),true);
	//DrawScuffedSquareCheck(Vec3(5332.795898f, -5195.612305f, 83.99975f), Vec3(0.5f, 0.75f, 2.0f),true);
	//DrawScuffedSquareCheck(Vec3(5336.999023f, -5178.898926f, 83.838768f), Vec3(0.75f, 2.25f, 2.0f),true);

	// second wave cops car spawn trigger
	DrawAngledZone(Vec3(5324.5811f, -5173.1362f, 87.0081f), Vec3(5334.1455f, -5195.7168f, 80.3420f), 10.0f, true);

	// Instafail triggers
	DrawScuffedSquareCheck(Vec3(5301.347168f, -5180.395020f, 84.018921f), Vec3(4.0f, 3.0f, 1.5f), true);
	DrawScuffedSquareCheck(Vec3(5323.853516f, -5194.200195f, 93.518600f), Vec3(41.0f, 36.0f, 13.0f), true);

	// road 
	//  IS_ENTITY_AT_COORD(playerPedID, <<5473.072266, -5128.806152, 80.067757>>, <<56.0, 43.0, 5.0>>)
	DrawScuffedSquareCheck(Vec3(5473.072266f, -5128.806152f, 80.067757f), Vec3(56.0f, 43.0f, 5.0f), true);

	//train cutscenee
	//IS_ENTITY_IN_ANGLED_AREA(vehCar, <<3629.041016, -4750.028809, 109.965340>>, <<3435.589355, -4728.467285, 130.489777>>, 175.0)
	DrawAngledZone(Vec3(3629.041016f, -4750.028809f, 109.965340f), Vec3(3435.589355f, -4728.467285f, 130.489777f), 175.0f, true);
}

static void Script_Armenian1()
{
	//Lamar and Franklin race end
	DrawAngledZone(Vec3(13.991759f, -675.822205f, 31.484550f), Vec3(20.466270f, -688.156616f, 34.484550f), 4.0f, true);

	//Bypass checks - In case the player lost the cops after the triggers from the left and Right entrance to simeon
	/*
	DrawAngledZone(Vec3(-26.301994f, -1092.126343f, 25.433983f), Vec3(-34.196312f, -1089.121704f, 28.434271f), 3.0f, false);
	DrawAngledZone(Vec3(-32.192822f, -1086.231201f, 25.434008f), Vec3(-35.282406f, -1094.512451f, 28.434353f), 1.5f, false);
	DrawAngledZone(Vec3(-26.855288f, -1088.089111f, 25.433941f), Vec3(-28.949333f, -1094.194946f, 28.434189f), 1.5f, false);
	DrawAngledZone(Vec3(-21.093651f, -1085.448242f, 25.606892f), Vec3(-32.265572f, -1081.161133f, 28.401917f), 7.75f, false);
	*/
	DrawAngledZone(Vec3(-11.135968f, -1091.418945f, 25.422077f), Vec3(-24.348146f, -1126.346069f, 29.784176f), 13.0f, true);
	DrawAngledZone(Vec3(-58.523495f, -1072.446289f, 25.512421f), Vec3(-47.367687f, -1076.728027f, 28.776825f), 14.75f, true);

	//Simeon
	// DrawAngledZone(Vec3(-37.614616f, -1108.703613f, 25.455927f), Vec3(-38.443035f, -1110.918457f, 27.436029f), 2.25f, false); - Front door
	//DrawAngledZone(Vec3(-38.590809f, -1095.906860f, 25.422304f), Vec3(-51.048512f, -1100.539063f, 27.422304f), 10.5f, false); - Simeon 
	DrawAngledZone(Vec3(-26.404922f, -1086.267212f, 25.573242f), Vec3(-31.385181f, -1084.527832f, 27.573242f), 2.25f, true);
}

static void Script_Armenian2() {
	/*CONST_FLOAT LOCATE_SIZE_ANY_MEANS		6.0
		CONST_FLOAT LOCATE_SIZE_ON_FOOT_ONLY	4.0
		CONST_FLOAT	LOCATE_SIZE_HEIGHT			2.0
		CONST_FLOAT LOCATE_SIZE_MISSION_TRIGGER	2.75*/
		// VECTOR g_vAnyMeansLocate	= << LOCATE_SIZE_ANY_MEANS, LOCATE_SIZE_ANY_MEANS, LOCATE_SIZE_HEIGHT >>
		// VECTOR g_vOnFootLocate = << LOCATE_SIZE_ON_FOOT_ONLY, LOCATE_SIZE_ON_FOOT_ONLY, LOCATE_SIZE_HEIGHT >>
		// IS_ENTITY_AT_COORD(player_ped_id(),<<-1141.0928, -1576.7594, 3.3048>>,g_vAnyMeansLocate,true)

		// Arriving at alley
	DrawScuffedSquareCheck(Vec3(-1141.0928f, -1576.7594f, 3.3048f), Vec3(6.0f, 6.0f, 2.0f), true);

	// Arriving at car wash
	DrawScuffedSquareCheck(Vec3(-6.62f, -1393.36f, 28.32f), Vec3(4.0f, 4.0f, 2.0f), false);
}

static void Script_Armenian3() {

	// =-= Go to Mansion tests
	//around the Mansion
	//DrawScuffedSquareCheck(Vec3(-809.689331f, 167.738159f, 80.499176f), Vec3(55.0f, 35.0f, 25.0f), false); 
	//Wall tests
	/*DrawAngledZone(Vec3(-847.812927f, 182.998001f, 66.362663f), Vec3(-811.278198f, 180.250854f, 81.592949f), 30.0f, false);
	DrawAngledZone(Vec3(-843.973389f, 159.883316f, 65.309196f), Vec3(-809.686829f, 159.163422f, 76.786880f), 22.0f, false);
	DrawAngledZone(Vec3(-818.617554f, 161.953323f, 59.905128f), Vec3(-765.113220f, 168.867889f, 104.219299f), 40.0f, false);
	DrawAngledZone(Vec3(-772.186829f, 136.951172f, 65.974167f), Vec3(-772.276367f, 151.575790f, 101.474510f), 20.5f, false);
	DrawAngledZone(Vec3(-823.994324f, 190.366592f, 71.401894f), Vec3(-773.834778f, 189.717102f, 107.483505f), 10.0f, false);
	DrawAngledZone(Vec3(-776.787048f, 185.335403f, 71.335213f), Vec3(-770.144592f, 190.454834f, 107.858650f), 10.0f, false);
	DrawAngledZone(Vec3(-850.643372f, 145.596069f, 58.643929f), Vec3(-783.199402f, 146.322983f, 97.667755f), 10.0f, false);*/

	// =-=-=- Window climb
	//IS_ENTITY_IN_ANGLED_AREA(PLAYER_PED_ID(), << -801.670105, 166.004623, 74.841263 >> , << -802.518921, 168.107956, 79.740738 >> , 2.0
	DrawAngledZone(Vec3(-801.670105f, 166.004623f, 74.841263f), Vec3(-802.518921f, 168.107956f, 79.740738f), 2.0f, false);

	// =-=-=- Crash Car at Simeon's
	// Line 9084 in Armenian3.sch
	// Overcomplicated check. not doing it.
}
static void Script_Franklin0() {
	//IS_ENTITY_AT_COORD(PLAYER_PED_ID(), GET_HOOD_VECTOR(HVEC_BAD_GUY_HANGOUT), << 5, 5, LOCATE_SIZE_HEIGHT >> , TRUE)
	// //<<363.74927, 323.37021, 102.71001>>

	// IF IS_ENTITY_AT_ENTITY(PLAYER_PED_ID(), runnerChaseProperties.ped, <<7,7,7>>)
	// voltar depois com a opção de pegar a area do ped.

	//IS_ENTITY_IN_AREA(chopPed, << 526.3, -685.8, -20 >> , << 534.8, -665.1, 100 >> , FALSE, FALSE)
	//Chop train sniffing
	//GRAPHICS::DRAW_BOX(526.3f, -685.8f, -20.0f, 534.8f, -665.1f, 100.0f,0,0,255,35);
	DrawBox(Vec3(526.3f, -685.8f, -20.0f), Vec3(534.8f, -665.1f, 100.0f), GameColor(0, 0, 255, 35), true);

	//chase - Triggers that is needed for cutscene skip?
	//IS_ENTITY_IN_AREA(PLAYER_PED_ID(), <<394.53989, -869.11523, 20.28647>>, <<652.68286, -666.74109, 54.06824>>
	DrawBox(Vec3(652.68286f, -666.74109f, 54.06824f), Vec3(394.53989f, -869.11523f, 20.28647f), GameColor(255, 0, 255, 35), false);
	// IS_ENTITY_IN_AREA(PLAYER_PED_ID(), <<277.28827, -831.68213, 20.24340>>, <<390.69489, -646.09259, 54.50041>>
	DrawBox(Vec3(390.69489f, -646.09259f, 54.50041f), Vec3(277.28827f, -831.68213f, 20.24340f), GameColor(255, 0, 255, 35), false);
	//GRAPHICS::_0x23BA6B0C2AD7B0D3(true);

	//arriving at D's hangout
	DrawScuffedSquareCheck(Vec3(363.74927f, 323.37021f, 102.71001f), Vec3(5.0f, 5.0f, 2.0f), false);

	// 		/*HVEC_REC		RETURN << -216.77869, -1494.96509, 30.23503 >> */
	//last drive to
	//IS_ENTITY_AT_COORD(PLAYER_PED_ID(), GET_HOOD_VECTOR(HVEC_REC), g_vAnyMeansLocate, TRUE)
	DrawScuffedSquareCheck(Vec3(-216.77869f, -1494.96509f, 30.23503f), Vec3(6.0f, 6.0f, 2.0f), true); // NOT WORKING
}
static void Script_Franklin1() {
	//Leaving groove street without lamar and trevor.
	DrawAngledZone(Vec3(49.515247f, -1892.616577f, 14.793283f), Vec3(115.109360f, -1961.272461f, 28.311836f), 84.0f, true);
	DrawAngledZone(Vec3(53.815639f, -1893.396484f, 14.593086f), Vec3(-42.446770f, -1812.097900f, 33.587776f), 82.0f, true);
	DrawAngledZone(Vec3(-46.707905f, -1871.813354f, 21.337345f), Vec3(-30.788389f, -1852.596924f, 28.817188f), 24.0f, true);
	DrawAngledZone(Vec3(732.77f, -2621.94f, -1.72f), Vec3(731.85f, -2562.83f, 16.46f), 36.0f, true);
}

//Family
Vehicle fa1_boatTrailer = 0;
Ped fa1_franklinPed = 0;
static void Script_Family1()
{
	// =-=-=- Drive to location where the boat is
	//vMissionLocation = << -2151.2722, -268.0492, 12.8775 >>
	DrawAngledZone(Vec3(-2118.952637f, -224.515121f, 13.085728f), Vec3(-2150.723145f, -279.301636f, 24.510395f), 53.5f, false);
	DrawAngledZone(Vec3(-2206.664307f, -368.100800f, 10.999576f), Vec3(-2370.630127f, -279.892090f, 16.897869f), 39.75f, false);
	//DrawScuffedSquareCheck(Vec3(-2151.2722f, -268.0492f, 12.8775f), Vec3(130.0f, 130.0f, 30.0f), false);

	//edge cases test
	//IF IS_ENTITY_IN_ANGLED_AREA(PLAYER_PED_ID(), << -2036.1, -356.0, 50.3 >> , << -2035.1, -382.8, 42.7 >> , 30.0) AND IS_SPHERE_VISIBLE(GET_ENTITY_COORDS(truckYachtPackerTrailer), 1.0) //Covering two parking lot balconies overlooking freeway
	//OR IS_ENTITY_IN_ANGLED_AREA(PLAYER_PED_ID(), << -1928.2, -425.3, 27.5 >> , << -2063.9, -634.6, 0.04 >> , 10.0) AND IS_SPHERE_VISIBLE(GET_ENTITY_COORDS(truckYachtPackerTrailer), 7.0) //From freeway onramp to beach
	//OR IS_ENTITY_IN_ANGLED_AREA(PLAYER_PED_ID(), << -2443.9, -248.3, 15.0 >> , << -2420.8, -226.7, 20.04 >> , 40.0) AND IS_SPHERE_VISIBLE(GET_ENTITY_COORDS(truckYachtPackerTrailer), 7.0) //From freeway going the same direction as the yacht (in case player goes over the mountains and attempts to flank the yacht)
	//OR IS_ENTITY_IN_ANGLED_AREA(PLAYER_PED_ID(), << -2222.1, -225.3, 90.0 >> , << -2430.8, -140.7, 10.04 >> , 10.0) AND IS_SPHERE_VISIBLE((GET_ENTITY_COORDS(truckYachtPackerTrailer) + << 0.0, 0.0, -8.0 >> ), 7.0) //Mountain ridge overlooking freeway
	//DrawAngledZone(Vec3(-2036.1f, -356.0f, 50.3f), Vec3(-2035.1f, -382.8f, 42.7f), 30.0f, false);
	//DrawAngledZone(Vec3(-1928.2f, -425.3f, 27.5f), Vec3(-2063.9f, -634.6f, 0.04f), 10.0f, false);
	//DrawAngledZone(Vec3(-2443.9f, -248.3f, 15.0f), Vec3(-2420.8f, -226.7f, 20.04f), 40.0f, false);
	//DrawAngledZone(Vec3(-2222.1f, -225.3f, 90.0f), Vec3(-2430.8f, -140.7f, 10.04f), 10.0f, false);

	/*DrawBasicText((char*)"Show Triggers", 0.5f, 0.95f, 0.4f);*/
	//from SSA
	// the game uses TR3 for the yatch
	if (fa1_boatTrailer == 0) {
		const int ARR_SIZE = 255;
		Vehicle vehInWorld[ARR_SIZE];
		int vehInWorldCount = worldGetAllVehicles(vehInWorld, ARR_SIZE);
		int loopveh = 0;
		bool foundveh = false;
		do {
			Vehicle vehTrailerTest = vehInWorld[loopveh];
			if (VEHICLE::IS_VEHICLE_MODEL(vehTrailerTest, GAMEPLAY::GET_HASH_KEY((char*)"TR3")) == TRUE) {
				if (ENTITY::IS_ENTITY_A_MISSION_ENTITY(vehTrailerTest)) {
					foundveh = true;
					fa1_boatTrailer = vehTrailerTest;
				}
			}
			loopveh++;
		} while (!foundveh && loopveh < vehInWorldCount);

	}
	else
	{
		if (VEHICLE::IS_VEHICLE_MODEL(fa1_boatTrailer, GAMEPLAY::GET_HASH_KEY((char*)"TR3")) == FALSE) {
			fa1_boatTrailer = 0;
			return;
		}
	}

	PLAYER::GET_PLAYERS_LAST_VEHICLE();

	// Model Names:
	// Player_Zero = Michael
	// Player_One = Franklin
	// PLayer_Two = Trevor
	//https://docs.altv.mp/gta/articles/ped/models.html

	if (fa1_franklinPed == 0)
	{
		const int ARR_SIZE = 255;
		Vehicle pedInWorld[ARR_SIZE];
		int pedInWorldCount = worldGetAllPeds(pedInWorld, ARR_SIZE);
		int loopped = 0;
		bool foundped = false;
		do {
			Ped pedTest = pedInWorld[loopped];
			if (PED::IS_PED_MODEL(pedTest, GAMEPLAY::GET_HASH_KEY((char*)"Player_One")) == TRUE)
			{
				foundped = true;
				fa1_franklinPed = pedTest;
			}
			loopped++;
		} while (!foundped && loopped < pedInWorldCount);

	}
	else {
		if (PED::IS_PED_MODEL(fa1_franklinPed, GAMEPLAY::GET_HASH_KEY((char*)"Player_One")) == FALSE) {
			fa1_franklinPed = 0;
		}
	}

	// Show distance between franklin and boat for early jump
	if (fa1_franklinPed != 0 && fa1_boatTrailer != 0)
	{
		if (!ENTITY::IS_ENTITY_ATTACHED_TO_ENTITY(fa1_boatTrailer, fa1_franklinPed))
		{
			Vec3 franPos = ENTITY::GET_ENTITY_COORDS(fa1_franklinPed, true);
			Vec3 boatPos = ENTITY::GET_ENTITY_COORDS(fa1_boatTrailer, true);
			float dis = SYSTEM::VDIST(franPos.x, franPos.y, franPos.z, boatPos.x, boatPos.y, boatPos.z);
			if (dis < 20.0f)
			{
				GRAPHICS::DRAW_LINE(franPos.x, franPos.y, franPos.z, boatPos.x, boatPos.y, boatPos.z, 0, 255, 0, 255);
				//DrawBasicTextWithColor((char*)"Early animation triggered", 0.4f, 0.90f, 0.4f, GameColor(0, 180, 0));
			}
			else
			{
				GRAPHICS::DRAW_LINE(franPos.x, franPos.y, franPos.z, boatPos.x, boatPos.y, boatPos.z, 255, 0, 0, 255);
				//DrawBasicTextWithColor((char*)"Get closer for early animation", 0.4f, 0.90f, 0.4f, GameColor(180, 0, 0));
			}
		}
	}

	// Show Zones for Franklin to jump
	if (fa1_boatTrailer != 0)
	{
		Vec3 vSubZone = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(fa1_boatTrailer, 0.0, -10.0, 0.0);
		DrawScuffedSquareCheck(vSubZone, Vec3(2.3f, 2.3f, 2.3f), false);
		Vec3 vSubZone1 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(fa1_boatTrailer, 0.0, -11.0, 0.0);
		DrawScuffedSquareCheck(vSubZone1, Vec3(2.3f, 2.3f, 2.3f), false);
		Vec3 vSubZone2 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(fa1_boatTrailer, 0.0, -13.0, 0.0);
		DrawScuffedSquareCheck(vSubZone2, Vec3(2.3f, 2.3f, 2.3f), false);
		Vec3 vSubZone3 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(fa1_boatTrailer, 0.0, -14.0, 0.0);
		DrawScuffedSquareCheck(vSubZone3, Vec3(2.3f, 2.3f, 2.3f), false);
	}

	//Franklin trigger; (if player don't get early animation)
	//IS_ENTITY_IN_ANGLED_AREA(PLAYER_PED_ID(), << -1840.488770, -724.232849, 7.849245 >> , << -1738.859009, -591.090881, 32.377640 >> , 27.500000
	//DrawAngledZone(Vec3(-1840.488770f, -724.232849f, 7.849245f), Vec3(-1738.859009f, -591.090881f, 32.377640f), 27.5f, false);

	// Arrive at LS customs
	// IS_PLAYER_AT_LOCATION_WITH_BUDDIES_IN_VEHICLE(sLocatesData, <<-1104.3127, -1975.6726, 12.0489>>, <<3.9, 3.9, 2.25>>, TRUE, pedFranklin(), NULL, NULL, carPlayersJeep, "YB_OBJ5", "CMN_FLEAVE", "", "", "CMN_FLEAVE", "YB_GETIN", "CMN_GENGETBCK", FALSE, TRUE)
	DrawScuffedSquareCheck(Vec3(-1104.3127f, -1975.6726f, 12.0489f), Vec3(3.9f, 3.9f, 2.25f), true);

}
static void Script_Family3()
{
	//Go to Balcony position
	// vUnderBalconyPosition= << -991.65, 645.38, 138.64 >> 4.0, 8.0, LOCATE_SIZE_HEIGHT
	DrawScuffedSquareCheck(Vec3(-991.65, 645.38, 138.64), Vec3(4.0f, 8.0f, 2.0f), false);
}
static void Script_Family4()
{
	//<< -219.5123, -2037.8512, 26.9 >> , << 0.01, 0.01, LOCATE_SIZE_HEIGHT >> 
	//DrawScuffedSquareCheck(Vec3(-219.5123f, -2037.8512f, 26.9f), Vec3(0.01f, 0.01f, 2.0f), true); // just the yellow dot position at the stadium

	DrawAngledZone(Vec3(-248.824f, -2021.740f, 28.944f), Vec3(-258.934f, -2033.879f, 31.958f), 4.5f, true); //Stadium doors.

	//The get out of vehicle zone at the stadium
	//func float calculate_angled_area_width_for_vehicle_slow_down() ADAPTED FROM R* CODE TO C++
	float min_distance = 0.5f;
	float max_distance = 6.7f;
	float min_velocity = 3.0f;
	float max_velocity = 12.0f;
	float area_scale_multiplier = 0;
	float area_width = 0;

	Vehicle players_car;

	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
	{
		players_car = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);

		float current_velocity = ENTITY::GET_ENTITY_SPEED(players_car);

		if (current_velocity < 3.0) {
			current_velocity = 3.0;
		}
		float area_scale_multiplier = (current_velocity - min_velocity) / (max_velocity - min_velocity);
		area_width = min_distance + ((max_distance - min_distance) * area_scale_multiplier);
	}
	else {
		area_width = 6.7f;
	}
	DrawAngledZone(Vec3(-221.051f, -2039.714f, 26.620f), Vec3(-217.358f, -2033.186f, 29.620f), area_width, true);

}
static void Script_Family6() {
	// Beam Machine
	DrawScuffedSquareCheck(Vec3(-630.973633f, 255.312454f, 82.295090f), Vec3(6.0f, 4.0f, 2.0f));
	DrawAngledZone(Vec3(-629.12653f, 242.94969f, 80.89502f), Vec3(-621.01813f, 242.78120f, 83.0f), 3.5f, true);

	// Tattoo shop
	DrawAngledZone(Vec3(302.371002f, 172.648605f, 102.820877f), Vec3(294.813934f, 175.443298f, 106.071678f), 5.0f, true);
	DrawAngledZone(Vec3(321.037598f, 178.496094f, 102.441963f), Vec3(319.134644f, 173.429993f, 106.266571f), 6.0f, true);

	// Friedlander
	DrawAngledZone(Vec3(-1894.675781f, -567.717773f, 10.614735f), Vec3(-1906.339966f, -557.923096f, 13.812391f), 8.0f, true);

	// Drive Home
	DrawAngledZone(Vec3(-826.59497f, 177.70781f, 68.15234f), Vec3(-823.29541f, 183.13243f, 75.85941f), 5.54f, true);

}

static void Script_Lester1()
{
	DrawAngledZone(Vec3(-1043.331787f, -231.809570f, 37.764164f), Vec3(-1045.451904f, -224.953644f, 40.952034f), 4.625f, true);
}
static void Script_jewelry_setup1()
{
	//VECTOR vParkingSpace = << -659.0, -270.9, 35 >>
	//VECTOR vWareHouse = << 718, -981.7, 23.5 >>

	//Parking at vangelico
	//DrawScuffedSquareCheck(Vec3(-659.0f, -270.9f, 35.0f),Vec3(7.0f,7.0f,4.0f));
	//DrawAngledZone(Vec3(-665.884460f, -259.601532f, 34.244663f), Vec3(-652.584473f, -283.042877f, 38.005383f), 11.0f, true);
	DrawAngledZone(Vec3(-661.583679f, -267.478821f, 34.398075f), Vec3(-657.259460f, -274.933685f, 37.457153f), 6.25f, true);
	// Slowdown for the store
	DrawAngledZone(Vec3(-631.130676f, -237.373398f, 36.556953f), Vec3(-617.120361f, -227.004135f, 39.915310f), 17.75f, true);
	DrawAngledZone(Vec3(-641.029663f, -233.201431f, 35.846058f), Vec3(-630.599304f, -247.389801f, 39.151962f), 12.0f, true);


}
static void Script_Jewelry_heist()
{
	//	vPos1 = <<1038.0, -226.0, 40.0>>
	//	vPos2 = << 1008.0, -231.5, 55.0 >>
	// IF bEntering3rdPersonPushin = FALSE
	// IF IS_ENTITY_IN_ANGLED_AREA(PLAYER_PED_ID(), vPos1, vPos2, 12.5)
	DrawAngledZone(Vec3(1038.0f, -226.0f, 40.0f), Vec3(1008.0f, -231.5f, 55.0f), 5.0f, true);
}

static void Script_Lamar1() {

	// top of stairs at the factory
	DrawAngledZone(Vec3(-609.949890f, -1619.683838f, 32.010139f), Vec3(-614.033020f, -1618.932495f, 34.509949f), 2.75f, true);

	//Checkpoint
	DrawAngledZone(Vec3(-568.24f, -1607.67f, 30.48f), Vec3(-577.45f, -1606.81f, -25.76f), 16.75, true);
}
static void Script_Trevor1() {
	//Arrive at bike gang
	DrawAngledZone(Vec3(1934.793823f, 4634.039063f, 38.488747f), Vec3(1966.492676f, 4647.708008f, 42.865807f), 15.5f, true);
	DrawAngledZone(Vec3(1949.083496f, 4634.543457f, 38.653030f), Vec3(1923.866089f, 4645.178711f, 42.060905f), 15.5f, true);

	// Fight at trailer park
	DrawAngledZone(Vec3(-4.104918f, 3693.259277f, 37.892815f), Vec3(29.597843f, 3712.840088f, 41.891338f), 34.25f, true);
	//DrawAngledZone(Vec3(23.309280f, 3706.247803f, 38.455639f), Vec3(34.667908f, 3684.702148f, 43.454075f), 25.5f, true);

}
Vehicle Tr2_ronPlane = 0;
Ped Tr2_ronPed = 0;
Blip Tr2_ronBlip = 0;
static void Script_Trevor2()
{
	//Arrive at Water Tower
	//DrawScuffedSquareCheck(Vec3(1580.0f, 3364.2393f, 37.5f), Vec3(30.0f, 30.0f, 30.0f), true);
	//IS_PLAYER_AT_LOCATION_ON_FOOT(sLocatesData, << 1574.8378, 3363.7754, 47.6348 >> , << 0.1, 0.1, LOCATE_SIZE_HEIGHT >> , TRUE, "SJC_TOWERG")
	DrawAngledZone(Vec3(1572.984253f, 3363.694336f, 50.384834f), Vec3(1576.692139f, 3363.694824f, 47.534836f), 4.5f, false);

	DoPedCheckUpdate(Tr2_ronPed, (char*)"IG_NERVOUSRON");

	if (PED::IS_PED_IN_ANY_VEHICLE(Tr2_ronPed, true))
	{
		Tr2_ronPlane = PED::GET_VEHICLE_PED_IS_IN(Tr2_ronPed, false);
		if (!VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(Tr2_ronPlane)))
		{
			Tr2_ronPlane = 0;
		}
	}
	else
	{
		Tr2_ronPlane = 0;
	}

	//if (Tr2_ronPlane != 0) {
	//	Vec3 planeCoords = ENTITY::GET_ENTITY_COORDS(Tr2_ronPlane, true);
	//	//if (planeCoords.z > 1.0f)
	//	{
	//		if (UI::DOES_BLIP_EXIST(Tr2_ronBlip))
	//			UI::REMOVE_BLIP(&Tr2_ronBlip);
	//		Tr2_ronBlip = UI::ADD_BLIP_FOR_RADIUS(planeCoords.x, planeCoords.y, planeCoords.z, 500.0f);
	//	}
	//}
	//else
	//{
	//	if (UI::DOES_BLIP_EXIST(Tr2_ronBlip))
	//		UI::REMOVE_BLIP(&Tr2_ronBlip);
	//}
	if (Tr2_ronPlane != 0) {
		Vec3 planeCoords = ENTITY::GET_ENTITY_COORDS(Tr2_ronPlane, true);
		//if (planeCoords.z > 1.0f)
		{
			if (!UI::DOES_BLIP_EXIST(Tr2_ronBlip))
				Tr2_ronBlip = UI::ADD_BLIP_FOR_RADIUS(planeCoords.x, planeCoords.y, planeCoords.z, 700.0f);
			UI::SET_BLIP_COORDS(Tr2_ronBlip, planeCoords.x, planeCoords.y, planeCoords.z);
			UI::SET_BLIP_COLOUR(Tr2_ronBlip, 2);
			UI::SET_BLIP_ALPHA(Tr2_ronBlip, 25);
		}
	}
	else
	{
		if (UI::DOES_BLIP_EXIST(Tr2_ronBlip))
			UI::REMOVE_BLIP(&Tr2_ronBlip);
	}

	DrawScuffedSquareCheck(Vec3(-3345.1748f, 3001.2778f, -0.0897f), Vec3(175.0f, 175.0f, 800.0f), true);

}
static void Script_Trevor3()
{
	// Friends Reunited

	// Los Santos view cutscene
	DrawScuffedSquareCheck(Vec3(-1350.1467f, 725.3141f, 185.3831f), Vec3(4.0f, 4.0f, 2.0f), true);// if force car to stop trigger is hit, this is used.
	DrawScuffedSquareCheck(Vec3(-1350.1467f, 725.3141f, 185.3831f), Vec3(6.0f, 6.0f, 2.0f), true);
	DrawAngledZone(Vec3(-1373.543823f, 728.595947f, 182.160049f), Vec3(-1337.614624f, 738.550415f, 187.760025f), 12.125f, true); // Force car to stop trigger
}
static void Script_Trevor4()
{
	DrawScuffedSquareCheck(Vec3(122.50f, -1323.80f, 28.36f), Vec3(6.0f, 6.0f, 2.0f));
}
Blip Fbi1_franklinCopsBlip = 0;
static void Script_FBI1() {
	// Dead Man Walking

	//Ending trigger at franklin.
	DrawAngledZone(Vec3(1586.747f, -1943.608f, 90.969f), Vec3(1611.747f, -1943.609f, 110.969f), 42.0f, true);

	//Cops trigger
	Ped franklinPed = 0;
	DoPedCheckUpdate(franklinPed, (char*)"Player_One");
	if (franklinPed != 0)
	{
		Vec3 franklinCoords = ENTITY::GET_ENTITY_COORDS(franklinPed, true);
		//if (planeCoords.z > 1.0f)
		{
			if (!UI::DOES_BLIP_EXIST(Fbi1_franklinCopsBlip))
				Fbi1_franklinCopsBlip = UI::ADD_BLIP_FOR_RADIUS(franklinCoords.x, franklinCoords.y, franklinCoords.z, 45.0f);
			UI::SET_BLIP_COORDS(Fbi1_franklinCopsBlip, franklinCoords.x, franklinCoords.y, franklinCoords.z);
			UI::SET_BLIP_COLOUR(Fbi1_franklinCopsBlip, 1);
			UI::SET_BLIP_ALPHA(Fbi1_franklinCopsBlip, 50);
		}
	}
}
static void Script_FBI2() {
	//CONST_FLOAT HOVER_TARGET_MAX_HEIGHT 284.0
	//CONST_FLOAT HOVER_TARGET_MIN_HEIGHT 257.0
	//IS_ENTITY_IN_ANGLED_AREA(PLAYER_PED_ID(), << 102.560, -645.660, HOVER_TARGET_MIN_HEIGHT >> , << 116.050, -609.760, HOVER_TARGET_MAX_HEIGHT >> , 20.0, FALSE, FALSE)
	DrawAngledZone(Vec3(102.560f, -645.660f, 257.0f), Vec3(116.050f, -609.760f, 284.0f), 20.0f);
}
static void Script_FBI5A() {

	//VECTOR v_pipe_pos_above_water = << 3947.8379, 3862.7820, 0.5207 >>
	//VECTOR v_pipe_pos_under_water = << 3833.91113, 3667.39355, -24.9 >>

	//Arrive at location
	DrawScuffedSquareCheck(Vec3(3947.8379f, 3862.7820f, 0.5207f), Vec3(20.0f, 20.0f, 2.5f));
	//Player at pipes
	//IS_PLAYER_AT_ANGLED_AREA_ON_FOOT(locates_data, v_pipe_pos_under_water, <<3836.541, 3668.579, -24.876>>, <<3833.517, 3670.342, -21.276>>, 2.500000, true, "") 
	DrawAngledZone(Vec3(3836.541f, 3668.579f, -24.876f), Vec3(3833.517f, 3670.342f, -21.276f), 2.5f);

	// End of swiming section
	DrawAngledZone(Vec3(3531.911f, 3706.334f, 19.992f), Vec3(3528.027f, 3706.687f, 22.992f), 3.7f, true);
	DrawAngledZone(Vec3(3531.911f, 3706.334f, 19.992f), Vec3(3528.027f, 3706.687f, 22.992f), 1.0f, true);
	DrawAngledZone(Vec3(3528.667f, 3707.503f, 19.992f), Vec3(3525.471f, 3707.759f, 22.992f), 4.8f, true);

	int a[2048];
	worldGetAllObjects(a, 2048);
	//crate.model = PROP_LD_CONTAINER
	//crate.pos = << 3638.326, 3771.813, 28.930 >>
	Object container_prop = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(3638.326f, 3771.813f, 28.930f, 0.1f, GAMEPLAY::GET_HASH_KEY((char*)"PROP_LD_CONTAINER"), false, 0, 0);
	if (container_prop != 0)
	{
		// Container cutscene
		//if is_entity_in_angled_area(player_ped_id(), get_offset_from_entity_in_world_coords(crate.obj, <<-2.100, -3.000, -1.400>>), get_offset_from_entity_in_world_coords(crate.obj, <<2.100, -3.000, 1.600>>), 2.300)
		//or is_entity_in_angled_area(player_ped_id(), get_offset_from_entity_in_world_coords(crate.obj, << -0.75, -1.50, -1.400 >> ), get_offset_from_entity_in_world_coords(crate.obj, << 0.75, -1.500, 1.600 >> ), 2.300)
		Vec3 pos1A = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(container_prop, -2.100f, -3.000f, -1.400f);
		Vec3 pos1B = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(container_prop, 2.100f, -3.000f, 1.600f);
		DrawAngledZone(pos1A, pos1B, 2.3f);
		Vec3 pos2A = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(container_prop, -0.75f, -1.50f, -1.400f);
		Vec3 pos2B = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(container_prop, 0.75f, -1.500f, 1.600f);
		DrawAngledZone(pos2A, pos2B, 2.3f);

		//Chopper grab container
		// is_entity_in_angled_area(player_ped_id(), get_offset_from_entity_in_world_coords(crate.obj, << -3.500, 0.0, 2.5 >> ), get_offset_from_entity_in_world_coords(crate.obj, << 3.500, 0.0, 12.00 >> ), 7.8, false)
		Vec3 pos3A = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(container_prop, -3.500f, 0.0f, 2.5f);
		Vec3 pos3B = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(container_prop, 3.500F, 0.0F, 12.00f);
		DrawAngledZone(pos3A, pos3B, 7.8f);
	}
	//Conteiner drop off
	// truck.pos = <<1756.7319, 3271.8926, 40.2172>>
	// truck.heading = 16.1635
	// truck.model = hauler
	// trailer.pos = <<1756.7319, 3271.8926, 45.2172>>
	// trailer.heading = 16.1635
	// trailer.model = trflat

	//Vehicle truck;
	Vehicle trailer = 0;
	//DoVehCheckUpdate(truck, (char*)"hauler");
	DoVehCheckUpdate(trailer, (char*)"trflat");
	if (trailer != 0)
	{
		// truck_angled_area_l = get_offset_from_entity_in_world_coords(trailer.veh, <<-6.0, -0.5, 0.0>>) //-2.5
		Vec3 trailerOff1 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(trailer, 6.0f, -0.52f, 13.0f);
		// truck_angled_area_r = get_offset_from_entity_in_world_coords(trailer.veh, << 6.0, -0.52, 13.0 >> )
		Vec3 trailerOff2 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(trailer, -6.0f, -0.5f, 0.0f);
		DrawAngledZone(trailerOff1, trailerOff2, 15.3f);
	}

	// Land the heli
	//is_entity_at_coord(player_ped_id(), << 1730.1304, 3275.4885, 40.1240 >> , << 25.0, 25.0, LOCATE_SIZE_HEIGHT >> , true)
	DrawScuffedSquareCheck(Vec3(1730.1304f, 3275.4885f, 40.1240f), Vec3(25.0f, 25.0f, 2.0f));

	//Taking patricia to Madrazzo
	DrawScuffedSquareCheck(Vec3(1362.9181, 1156.9108, 112.7590), 2.5f);
}
static void Script_Chinese1()
{
	// on Wave 3, game does a few checks to decide when to give checkpoint 4.
	//first one is checking if player is inside this zone.
	DrawAngledZone(Vec3(-1373.543823f, 728.595947f, 182.160049f), Vec3(-1337.614624f, 738.550415f, 187.760025f), 12.125f, true);

	// or player Z coords is bellow < 25
}
//Docks Heist
static void Script_docks_setup()
{
	//Crates deposit area
	DrawAngledZone(Vec3(-92.523666f, -2461.800049f, 8.267201f), Vec3(-103.638123f, -2453.989258f, 4.520514f), 14.75f, true);
	//Not park zone
	//<<-1157.126465,-1520.959229,11.133599>> <<5.000000,5.500000,4.000000>>)
	//DrawScuffedSquareCheck(Vec3(-1157.126465f, -1520.959229f, 11.133599f), Vec3(5.0f, 5.0f, 5.0f));

}
static void Script_carsteal1()
{
	// Gas Station
	DrawAngledZone(Vec3(2558.925537f, 355.287384f, 107.121086f), Vec3(2598.119873f, 353.765930f, 121.994392f), 35.0f, true);

	// Race end
	DrawAngledZone(Vec3(2479.381104f, 4728.353027f, 23.323929f), Vec3(3015.382324f, 4846.076172f, 153.630157f), 450.0f, true);
}
static void Script_carsteal2()
{
	// Trigger for the gates
	float fStopRange;
	float fZSpeed;
	fZSpeed = ENTITY::GET_ENTITY_SPEED(PLAYER::GET_PLAYERS_LAST_VEHICLE());
	fStopRange = fZSpeed * 0.2;
	if (fStopRange < 3) {
		fStopRange = 3;
	}
	fStopRange += 2;
	//IF GET_DISTANCE_BETWEEN_ENTITY_AND_COORD(vehicle[vehHeist].id,<<-968.4945, -2798.2935, 12.9648>>) < fStopRange
	DrawScuffedSquareCheck(Vec3(-968.4945f, -2798.2935f, 12.9648f), fStopRange);

	//Drop off location
	// <<-990.14, -3016.53, 12.94>> g_vAnyMeansLocate <<6.0,6.0,2.0>>
	DrawScuffedSquareCheck(Vec3(-990.14f, -3016.53f, 12.94f), Vec3(6.0f, 6.0f, 2.0f));

}

static void Script_Solomon1()
{
	//Drive to place
	DrawAngledZone(Vec3(976.58124f, -1753.84753f, 33.0f), Vec3(965.19244f, -1753.20862f, 28.0f), 8.0f, true);

	//Rocco Heli zone
	DrawAngledZone(Vec3(908.51294f, -1699.99683f, 65.0f), Vec3(910.62000f, -1663.53870f, 50.0f), 18.0f, true);

	//Landing Area
	DrawAngledZone(Vec3(-1107.32568f, -461.81781f, 30.17446f), Vec3(-1089.89941f, -452.28159f, 36.5f), 20.0f, true);

	//Start last Cutscene
	DrawAngledZone(Vec3(-1055.51233f, -505.69760f, 34.0f), Vec3(-1009.98639f, -483.01324f, 45.93766f), 26.0f, true);

}
Vehicle mar1_plane = 0;
Blip mar1_planeFailBlip = 0;
static void Script_Martin1() {
	// Plane Fail zone
	//  GET_DISTANCE_BETWEEN_ENTITIES(vehTitan, PLAYER_PED_ID()) > 750
	DoVehCheckUpdate(mar1_plane, (char*)"SHAMAL");
	if (mar1_plane != 0)
	{
		Vec3 planeCoords = ENTITY::GET_ENTITY_COORDS(mar1_plane, true);
		if (!UI::DOES_BLIP_EXIST(mar1_planeFailBlip)) {
			mar1_planeFailBlip = UI::ADD_BLIP_FOR_RADIUS(0, 0, 0, 750);
		}
		UI::SET_BLIP_COORDS(mar1_planeFailBlip, planeCoords.x, planeCoords.y, planeCoords.z);
		UI::SET_BLIP_COLOUR(mar1_planeFailBlip, 2);
		UI::SET_BLIP_ALPHA(mar1_planeFailBlip, 25);
	}
	else {
		RemoveUselessBlip(mar1_planeFailBlip);
	}

	//Plane door
	DrawAngledZone(Vec3(2807.429199f, 4802.980469f, 45.975845f), Vec3(2806.673584f, 4799.813477f, 48.226929f), 3.5f, true);

	//Final Drive
	DrawScuffedSquareCheck(Vec3(1216.6f, 1868.1f, 78.1f), 5.0f);
}
static void Script_Exile2()
{
	//Crash site <<-1874.7830, 4484.7104, 24.2898>>
	// start call
	DrawScuffedSquareCheck(Vec3(-1874.7830f, 4484.7104f, 24.2898f), Vec3(45.0f, 45.0f, 25.0f));
	// Finale
	DrawScuffedSquareCheck(Vec3(1731.0f, 3273.7f, 40.5f), Vec3(20.0f, 20.0f, 2.0f), true);
}
Vehicle ex2_freight = 0;
static void Script_Exile3() {
	//CASE TCVEC_BRIDGE_TRIGGER
	//RETURN << -502.94, 4426.99, 30.93 >>

	DoVehCheckUpdate(ex2_freight, (char*)"freight");
	if (ex2_freight != 0)
	{
		if (VEHICLE::IS_VEHICLE_MODEL(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), GAMEPLAY::GET_HASH_KEY((char*)"sanchez"))) {
			Vec3 trainPos = ENTITY::GET_ENTITY_COORDS(ex2_freight, true);
			trainPos.z += 2.0f;
			DrawScuffedSquareCheck(trainPos, Vec3(16.0f, 16.0f, 2.0f));
		}
	}
	// Talking trigger at small bridge
	// game is waiting for this dialog to end and Michael y pos > 4402.55 to start the train crash cutscene.
	// DrawScuffedSquareCheck(Vec3(-502.94, 4426.99, 30.93), Vec3(400.0f,400.0f,20.0f));

	// Beach stop
	DrawAngledZone(Vec3(-2406.739f, 4235.841f, -10.0f), Vec3(-2474.212f, 4272.588f, 15.0f), 180.0f, true);

	// Get close to cars to start cutscene
	//IS_ENTITY_AT_ENTITY(PLAYER_PED_ID(), trainCrashVehicle[TCV_BEACH_CAR], <<LOCATE_SIZE_ANY_MEANS, LOCATE_SIZE_ANY_MEANS, LOCATE_SIZE_HEIGHT>>)
	// TCV_BEACH_CAR <<-2430.87, 4260.73, 5.37>>
	//DrawScuffedSquareCheck(Vec3(-2430.87f, 4260.73f, 5.37f),Vec3(6.0f,6.0f,2.0f));
	//IS_ENTITY_AT_ENTITY(PLAYER_PED_ID(), trainCrashVehicle[TCV_BEACH_TRUCK], << LOCATE_SIZE_ANY_MEANS, LOCATE_SIZE_ANY_MEANS, LOCATE_SIZE_HEIGHT >> )
	// TCV_BEACH_TRUCK <<-2433.61, 4249.80, 5.78>>
	//DrawScuffedSquareCheck(Vec3(-2433.61f, 4249.80f, 5.78f),Vec3(6.0f, 6.0f, 2.0f));
	//IS_ENTITY_AT_COORD(PLAYER_PED_ID(), GET_TRAIN_CRASH_VECTOR(TCVEC_BEACH_VEHICLES), << 13, 13, LOCATE_SIZE_HEIGHT >> )
	// TCVEC_BEACH_VEHICLES <<-2431.49976, 4255.95166, 5.46902>>
	DrawScuffedSquareCheck(Vec3(-2431.49976f, 4255.95166f, 5.46902f), Vec3(13, 13, 2.0f));

}
static void Script_rural_bank_heist()
{
	//VECTOR vBankCoordinates = << -137.9823, 6460.2036, 30.5964 >>
	//VECTOR vBankParkingLocation = << -126.82808, 6466.89600, 30.44632 >>
	// Drop point franklin
	DrawAngledZone(Vec3(10.543437f, 6643.602051f, 29.789328f), Vec3(8.626600f, 6649.545410f, 33.988613f), 7.25f, true);

	//Bank Parking
	//IS_ENTITY_AT_COORD(arrivalBurrito, vBankParkingLocation, <<4.25, 4.25, 3.0>>, FALSE)
	DrawScuffedSquareCheck(Vec3(-126.82808f, 6466.89600f, 30.44632f), Vec3(4.25f, 4.25f, 3.0f));

	//Bank entrance
	// GET_DISTANCE_BETWEEN_ENTITY_AND_COORD(PLAYER_PED_ID(), <<-109.622, 6467.932, 30.712>>) < 15.5

	//Rob the bank
	//DrawAngledZone(Vec3(-114.719948f, 6474.139160f, 30.651968f), Vec3(-109.286812f, 6468.747559f, 33.392658f), 1.5f, true);


}
static void Script_finale_heist1()
{
	//VECTOR		vFRONT_BANK = << -7.8156, -741.8263, 43.1570 >>
	//FLOAT		fHEADING_FRONT = 69.4048
	//VECTOR		vBACK_BANK = << -110.60891, -636.62433, 35.10708 >>
	//FLOAT		fHEADING_BACK = 343.3254
	//VECTOR		vAIRFIELD = << 1768.99805, 3279.52417, 40.37643 >>
	//Vector		vTRUCK_START = << 1147.39539, -1718.47021, 34.71170 >>
	//Vector		vHOLE = << 25.30480, -635.79169, 30.30575 >>

	//Front of the bank
	DrawAngledZone(Vec3(-2.55928f, -743.97137f, 45.0f), Vec3(-14.34047f, -739.44629f, 43.15934f), 4.46f);
	//Back of the bank
	DrawScuffedSquareCheck(Vec3(-110.60891f, -636.62433f, 35.10708f), Vec3(3.0f, 3.0f, 2.0f));
	//Drive to airfield
	DrawScuffedSquareCheck(Vec3(1768.99805f, 3279.52417f, 40.37643f), 14.9f);
	//Fly to trucks
	DrawScuffedSquareCheck(Vec3(1147.39539f, -1718.47021f, 34.71170f), 379.9f);
	//Fly over big hole for 30 seconds.
	DrawAngledZone(Vec3(30.85060f, -616.74359f, 30.0f), Vec3(19.54452f, -648.75940f, 48.0f), 23.9f);
}
static void Script_Michael1() {
	// Drive to Airport
	// Start cutscene in car
	DrawAngledZone(Vec3(-1026.306519f, -2741.366699f, 18.669733f), Vec3(-1046.329590f, -2730.003174f, 23.169733f), 18.0f, true);
	// Start cutscene on foot
	DrawAngledZone(Vec3(-1046.410645f, -2736.281982f, 18.670202f), Vec3(-1032.384521f, -2744.483154f, 23.170202f), 17.0f, true);
	// Parking at the airport
	DrawAngledZone(Vec3(-1047.949463f, -2649.732422f, 12.331177f), Vec3(-1027.131958f, -2661.605469f, 15.981733f), 7.0f, true);
	DrawAngledZone(Vec3(-1037.549438f, -2680.126465f, 12.481735f), Vec3(-1047.742188f, -2670.507324f, 15.831176f), 7.0f, true);
	DrawAngledZone(Vec3(-1057.455078f, -2653.344482f, 12.331177f), Vec3(-1047.972046f, -2671.035645f, 15.831175f), 7.0f, true);
	// Drive to Graveyard
	DrawScuffedSquareCheck(Vec3(3219.976563f, -4706.303223f, 112.876556f), Vec3(18.0f, 20.0f, 2.0f), true);
	// Arrive at Grave
	DrawAngledZone(Vec3(3240.262695f, -4574.430176f, 115.941917f), Vec3(3294.226807f, -4574.134766f, 120.941917f), 31.0f, true);
	// Fly home
	DrawAngledZone(Vec3(1719.452637f, 3254.324951f, 40.144505f), Vec3(1048.329956f, 3073.852295f, 50.704060f), 34.0f, true);

}
static void Script_Michael3() {
	// Get to steve
	DrawAngledZone(Vec3(-2150.920898f, 221.220291f, 183.401871f), Vec3(-2161.751709f, 245.579254f, 186.601868f), 4.0f, true);
	DrawAngledZone(Vec3(-2156.448975f, 238.033142f, 183.401871f), Vec3(-2157.896973f, 246.830170f, 186.601868f), 4.2f, true);
	DrawAngledZone(Vec3(-2155.382080f, 235.883850f, 183.401871f), Vec3(-2149.944092f, 226.999344f, 186.601868f), 4.2f, true);
	DrawAngledZone(Vec3(-2159.870117f, 227.744507f, 182.899124f), Vec3(-2165.927734f, 241.479019f, 186.578751f), 10.25f, true);
	DrawAngledZone(Vec3(-2151.657471f, 223.143082f, 183.401871f), Vec3(-2161.371826f, 217.807388f, 186.601868f), 6.0f, true);
	DrawAngledZone(Vec3(-2164.147949f, 252.144592f, 183.401871f), Vec3(-2157.978027f, 245.185471f, 186.601868f), 6.0f, true);
	DrawAngledZone(Vec3(-2170.639648f, 227.823486f, 183.351868f), Vec3(-2163.930420f, 231.200455f, 186.601868f), 9.0f, true);
	// After first shootout
	DrawAngledZone(Vec3(-2211.363525f, 238.609055f, 183.504868f), Vec3(-2229.537842f, 280.064331f, 186.604080f), 16.0f);
	// Escape
	DrawAngledZone(Vec3(-1436.604248f, -387.493134f, 34.620579f), Vec3(-1444.665283f, -376.455261f, 40.701355f), 8.75f, true);
	DrawAngledZone(Vec3(-1442.794312f, -381.582275f, 37.320213f), Vec3(-1470.929932f, -363.552002f, 42.366222f), 41.0f, true);

}
// =-=-=-=-=-=-=-=-=-=--= UPDATE =-=-=-=-=-=-=-=-=-=-

////Test
//Vector3 v1;
//Vector3 v2;
ULONGLONG timerForText =0;
ULONGLONG maxTimerForText = 1000;
bool ShowZones = false;
void Update()
{
	//if (IsKeyJustUp(VK_NUMPAD0))
	//{
	//	// search for marker blip
	//	int blipIterator = UI::_GET_BLIP_INFO_ID_ITERATOR();
	//	for (Blip i = UI::GET_FIRST_BLIP_INFO_ID(blipIterator); UI::DOES_BLIP_EXIST(i) != 0; i = UI::GET_NEXT_BLIP_INFO_ID(blipIterator))
	//	{
	//		if (UI::GET_BLIP_INFO_ID_TYPE(i) == 4)
	//		{
	//			Vec3 c = UI::GET_BLIP_INFO_ID_COORD(i);
	//			int e = PLAYER::PLAYER_PED_ID();
	//			if (PED::IS_PED_IN_ANY_VEHICLE(e, false)) {
	//				e = PED::GET_VEHICLE_PED_IS_IN(e, false);
	//			}

	//			ENTITY::SET_ENTITY_COORDS(e, c.x, c.y, 0, 0, 0, 0, 0);
	//			WAIT(100);
	//			//ground test
	//			float a;
	//			bool found = false;
	//			for (int i = 0; i < 5000; i += 100)
	//			{
	//				WAIT(100);
	//				if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(c.x, c.y, i, &a, true))
	//				{
	//					found = true;
	//					break;
	//				}
	//				if (i == 150) {
	//					ENTITY::SET_ENTITY_COORDS(e, c.x, c.y, 150, 0, 0, 0, 0);
	//					WAIT(100);
	//				}
	//				if (i == 200) {
	//					ENTITY::SET_ENTITY_COORDS(e, c.x, c.y, 200, 0, 0, 0, 0);
	//					WAIT(100);
	//				}
	//				if (i == 250) {
	//					ENTITY::SET_ENTITY_COORDS(e, c.x, c.y, 250, 0, 0, 0, 0);
	//					WAIT(100);
	//				}
	//				if (i == 500) {
	//					ENTITY::SET_ENTITY_COORDS(e, c.x, c.y, 500, 0, 0, 0, 0);
	//					WAIT(100);
	//				}
	//				if (i == 800) {
	//					ENTITY::SET_ENTITY_COORDS(e, c.x, c.y, 800, 0, 0, 0, 0);
	//					WAIT(100);
	//				}
	//				if (i == 1000) {
	//					ENTITY::SET_ENTITY_COORDS(e, c.x, c.y, 1000, 0, 0, 0, 0);
	//					WAIT(100);
	//				}
	//				if (i == 1500) {
	//					ENTITY::SET_ENTITY_COORDS(e, c.x, c.y, 1500, 0, 0, 0, 0);
	//					WAIT(100);
	//				}
	//			}
	//			//if didn't find it, fuck it, let the game deal with it.
	//			a = found ? a : -199;
	//			ENTITY::SET_ENTITY_COORDS(e, c.x, c.y, a, 0, 0, 0, 0);
	//			break;
	//		}
	//	}
	//}

	if (CONTROLS::IS_CONTROL_JUST_PRESSED(2,eControl::ControlFrontendDelete))
	{
		ShowZones = !ShowZones;
		timerForText = GetTickCount64();
	}
	if (GetTickCount64() < maxTimerForText + timerForText) {
		if (ShowZones)
		{
			DrawBasicText((char*)"ShowZones Enabled", 0.45f, 0.96f, 0.5f);
		}
		else {
			DrawBasicText((char*)"ShowZones Disabled", 0.45f, 0.96f, 0.5f);
		}
	}
	
	//DrawBox(Vec3(v1), Vec3(v2), GameColor(0, 255, 255, 255), true);
	//DrawScuffedSquareCheck(Vec3(-1156.341f, -1525.126f, 11.275f), Vec3(0.1f, 0.1f, 5.0f));

	// =-=-=-=-=-=-=-=-=-=-=-=-= MISSION SCRIPTS =-=-=-=-=-=-=-=-=-=
	if (ShowZones)
	{

		if (IsScriptActive((char*)"Prologue1"))		Script_Prologue1();
		// Simeon / Armenian
		if (IsScriptActive((char*)"Armenian1"))		Script_Armenian1();
		if (IsScriptActive((char*)"Armenian2"))		Script_Armenian2();
		if (IsScriptActive((char*)"Armenian3"))		Script_Armenian3();
		// Franklin
		if (IsScriptActive((char*)"Franklin0"))		Script_Franklin0();
		if (IsScriptActive((char*)"Franklin1"))		Script_Franklin1();
		// Family
		if (IsScriptActive((char*)"Family1"))		Script_Family1();
		if (IsScriptActive((char*)"Family3"))		Script_Family3();
		if (IsScriptActive((char*)"Family4"))		Script_Family4();
		if (IsScriptActive((char*)"Family6"))		Script_Family6();
		// Lester
		if (IsScriptActive((char*)"Lester1"))		Script_Lester1();
		// Jewel Heist
		if (IsScriptActive((char*)"jewelry_heist")) Script_Jewelry_heist();
		if (IsScriptActive((char*)"jewelry_setup1")) Script_jewelry_setup1();
		// Lamar
		if (IsScriptActive((char*)"Lamar1"))		Script_Lamar1();
		// Countryside / Trevor
		if (IsScriptActive((char*)"Trevor1"))		Script_Trevor1();
		if (IsScriptActive((char*)"Trevor2"))		Script_Trevor2();
		else
		{
			RemoveUselessBlip(Tr2_ronBlip);
		}
		if (IsScriptActive((char*)"Trevor3"))		Script_Trevor3();
		if (IsScriptActive((char*)"Trevor4"))		Script_Trevor4();
		// Chinese
		if (IsScriptActive((char*)"Chinese1"))		Script_Chinese1();
		// FBI
		if (IsScriptActive((char*)"FBI1"))			Script_FBI1();
		else
		{
			RemoveUselessBlip(Fbi1_franklinCopsBlip);
		}
		if (IsScriptActive((char*)"FBI2"))			Script_FBI2();
		if (IsScriptActive((char*)"FBI5A"))			Script_FBI5A();
		// Docks heist
		if (IsScriptActive((char*)"docks_setup"))	Script_docks_setup();
		// Car Steal
		if (IsScriptActive((char*)"carsteal1"))	Script_carsteal1();
		if (IsScriptActive((char*)"carsteal2"))	Script_carsteal2();
		// Solomon
		if (IsScriptActive((char*)"Solomon1"))	Script_Solomon1();
		// Martin
		if (IsScriptActive((char*)"Martin1"))	Script_Martin1();
		else
		{
			RemoveUselessBlip(mar1_planeFailBlip);
		}
		// Exile
		if (IsScriptActive((char*)"Exile2"))	Script_Exile2();
		if (IsScriptActive((char*)"Exile3"))	Script_Exile3();
		// Paleto Bay Heist
		if (IsScriptActive((char*)"rural_bank_heist"))	Script_rural_bank_heist();
		// Big Score Heist
		if (IsScriptActive((char*)"finale_heist1"))	Script_finale_heist1();
		// Michael
		if (IsScriptActive((char*)"Michael1"))	Script_Michael1(); //Bury
		if (IsScriptActive((char*)"Michael3"))	Script_Michael3(); //Wrap up
	}
}

// =-=-=-= MAIN =-=-=-=-=-=
void ScriptMain()
{
	while (true)
	{
		Update();
		WAIT(0);
	}

}