#pragma once
#include <string>
#include "I_EditorPannel.h"
#include <vector>
#include <map>

struct ProfilePlus;
struct FrameData;

class EditorFlameGraph : public I_EditorPannel
{
public:
	EditorFlameGraph();
	~EditorFlameGraph();
	virtual void Update() override;
private:
	typedef std::vector<FrameData> frameDataVector;
	const frameDataVector* mapToGetData;
	const unsigned long* currentFrame;
	const unsigned int numFramesToShowMainLoopTimeTaken = 10;
	const std::string mainUpdateProfileName = "Main Update";
};