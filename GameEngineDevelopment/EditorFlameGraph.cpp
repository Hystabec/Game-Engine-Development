#include "EditorFlameGraph.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <imgui_sdl.h>
#include <imgui_internal.h>

#include "Profiler.h"
#include "Profile.h"

#include <algorithm>

EditorFlameGraph::EditorFlameGraph()
{
	mapToGetData = &profiler->getStoredProfileMap();
	currentFrame = &profiler->getRefToCurrentFrame();
}

EditorFlameGraph::~EditorFlameGraph()
{
	mapToGetData = nullptr;
}

std::vector<std::vector<Sample>> sortLayers(const std::vector<Sample>* vecToSort, unsigned int numberOfLayers)
{
	std::vector<std::vector<Sample>> vecToReturn;

	for (int index = 0; index < numberOfLayers; index++)
	{
		std::vector<Sample> thisLoopVec;
		for (Sample thisSample : *vecToSort)
		{
			if (thisSample.profile.stackDepth == index)
			{
				thisLoopVec.push_back(thisSample);
			}
		}
		vecToReturn.push_back(thisLoopVec);
	}

	auto sortFunc = [](Sample& a, Sample& b) { return (a.profile.functionIDAbove < b.profile.functionIDAbove) ; };
	for (std::vector<Sample> currentVec : vecToReturn)
	{
		std::sort(currentVec.begin(), currentVec.end(), sortFunc);
	}

	return vecToReturn;
}

void recusiveAddChildrenToDrawList(ImDrawList* drawlist, std::vector<std::vector<Sample>>& sortedFrameData, ImVec2 startPos, unsigned int currentDepth, unsigned int parentID, float parentWidth, float layerHight)
{
	//all data should be passed by reference - to save memory and time
	//and needs ref to the draw list

	if (sortedFrameData.size() <= currentDepth)
		return;

	std::vector<Sample> ParentsChildren;
	for(Sample& Sam : sortedFrameData[currentDepth])
	{
		if (Sam.profile.functionIDAbove == parentID)
			ParentsChildren.push_back(Sam);
	}

	const ImU32 colOutlineBase = ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x7FFFFFFF;
	const ImU32 colBase = ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x77FFFFFF;

	float theseBoxWidths = parentWidth / ParentsChildren.size();
	float widthSoFar = 0;

	for (int currentIndex = 0; currentIndex < ParentsChildren.size(); currentIndex++)
	{
		const ImVec2 minPos = ImVec2(startPos.x + widthSoFar, startPos.y);
		const ImVec2 maxPos = ImVec2(startPos.x + widthSoFar + theseBoxWidths, startPos.y + layerHight);
		drawlist->AddRectFilled(
			minPos,
			maxPos,
			colBase,
			GImGui->Style.FrameRounding);
		drawlist->AddRect(minPos, maxPos, colOutlineBase);

		ImGui::RenderText(ImVec2(minPos.x + 5, minPos.y + 5), ParentsChildren[currentIndex].sampleName.c_str());
		std::string timeTakenAsString = std::to_string(ParentsChildren[currentIndex].profile.timeTaken);
		ImGui::RenderText(ImVec2(minPos.x + 5, minPos.y + 15), timeTakenAsString.c_str());

		ImVec2 childStartPoint = ImVec2(startPos.x + widthSoFar, startPos.y + layerHight);
		recusiveAddChildrenToDrawList(drawlist, sortedFrameData, childStartPoint, currentDepth+1, ParentsChildren[currentIndex].profile.functionID, theseBoxWidths, layerHight);

		widthSoFar += theseBoxWidths;
	}
}

void EditorFlameGraph::Update()
{
	static std::vector<unsigned long> averageMainLoopTimeTakenVector;
	static unsigned long averageMainLoopTimeTaken = 0;
	static bool filledVector = false;

	if (!filledVector)
	{
		//this will only happen once and needs to early return on the first frame as it will try to read data from a vector that is yet to be made
		//and will give a memory access violation if not here

		//populates the average time taken vector so that it can be used once there is some data in the profiler
		for (int index = 0; index < numFramesToShowMainLoopTimeTaken; index++)
		{
			averageMainLoopTimeTakenVector.push_back(0);
		}
		filledVector = true;
		return;
	}

	profiler->startProfile("EFlameGraphUpdate");
	ImGui::Begin("Profiler");

	if (!(profiler->ActiveStatus()))
	{
		//if the profiler is not active no flame graph can be generated
		//so nothing should be shown
		ImGui::Text("Profiler is not active");

		ImGui::End();
		profiler->endProfile("EFlameGraphUpdate");
		return;
	}

	static bool showAverageFrameTimeTotal = true;

	ImGui::Checkbox("Show average frame time", &showAverageFrameTimeTotal);

	if (showAverageFrameTimeTotal)
	{
		//will remove the first item in the list and will add the current main loop time to the back
		//this works on the idea that there is a profile being created for the main loop
		//if there isnt this will probably break;

		static bool showLiveTime = true;

		ImGui::Checkbox("Show live time", &showLiveTime);

		if (showLiveTime)
		{
			averageMainLoopTimeTaken -= *averageMainLoopTimeTakenVector.begin();
			averageMainLoopTimeTakenVector.erase(averageMainLoopTimeTakenVector.begin());

			auto currentFrameData = profiler->getCurrentFrameData();
			unsigned long timeTakenForLoop = 0;

			for (Sample& currentSample : currentFrameData.sampleVector)
			{
				//main loop will always have a stack deepth of 0
				if (currentSample.profile.stackDepth == 0)
				{
					timeTakenForLoop = currentSample.profile.timeTaken;
					break;
				}
			}

			averageMainLoopTimeTaken += timeTakenForLoop;
			averageMainLoopTimeTakenVector.push_back(timeTakenForLoop);
		}

		//this calculates the average of the stored times and will add then to the text on the window
		std::string test = std::to_string(averageMainLoopTimeTaken/numFramesToShowMainLoopTimeTaken);
		ImGui::Text((mainUpdateProfileName + ":").c_str());
		ImGui::Text(("Time taken(us) "+test).c_str());

		std::vector<float> testVec;
		
		for (int i = 0; i < averageMainLoopTimeTakenVector.size(); i++)
		{
			testVec.push_back((float)averageMainLoopTimeTakenVector[i]);
		}

		ImGui::MyPlotHistogram("##FrameData", testVec.data(), testVec.size());

	}

	static bool showingLiveFlameGraph = true;
	static unsigned long SnapShotStartFrame = 0;
	static unsigned long SnapShotEndFrame = 1;

	ImGui::Checkbox("View flame graph", &showingLiveFlameGraph);

	if (showingLiveFlameGraph)
	{
		static bool updatingFrameData = true;

		ImGui::Checkbox("Live updating", &updatingFrameData);

		static std::vector<Sample> currentFrameData;
		if (updatingFrameData)
			currentFrameData = profiler->getCurrentFrameData().sampleVector;


		unsigned int numberOfItemsInLayerZero = 0;

		for (Sample& vs : currentFrameData)
		{
			if (vs.profile.stackDepth == 0)
				numberOfItemsInLayerZero++;
		}

		std::vector<std::vector<Sample>> sortedFrameData = sortLayers(&currentFrameData, profiler->getCurrentFrameData().maxDepth);

		ImDrawList* drawlist = ImGui::GetCurrentWindow()->DrawList;
		ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
		ImVec2 canvas_sz = ImGui::GetContentRegionAvail();

		if (canvas_sz.x < 50.0f)
			canvas_sz.x = 50.0f;
		if (canvas_sz.y < 50.0f)
			canvas_sz.y = 50.0f;
		ImVec2 canvae_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
		drawlist->PushClipRect(canvas_p0, canvae_p1, true);

		//index 0 is width of layer 0, 1 is width of 1 etc
		std::vector<float> vecOfLayerWidths;
		float layerHights = canvas_sz.y / sortedFrameData.size();
		for (std::vector<Sample>& CS : sortedFrameData)
		{
			vecOfLayerWidths.push_back(canvas_sz.x / CS.size());
		}

		const ImU32 colOutlineBase = ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x7FFFFFFF;
		const ImU32 colBase = ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x77FFFFFF;

		float TotalBlockWidthSoFar = 0;
		//float TotalBlockHightSoFar = 0;

		for (int indexOf0 = 0; indexOf0 < sortedFrameData[0].size(); indexOf0++)
		{
			const ImVec2 minPos = ImVec2(canvas_p0.x + TotalBlockWidthSoFar, canvas_p0.y);
			const ImVec2 maxPos = ImVec2(canvas_p0.x + TotalBlockWidthSoFar + vecOfLayerWidths[0], canvas_p0.y + layerHights);
			drawlist->AddRectFilled(
				minPos,
				maxPos,
				colBase,
				GImGui->Style.FrameRounding);
			drawlist->AddRect(minPos, maxPos, colOutlineBase);

			ImGui::RenderText(ImVec2(minPos.x + 5, minPos.y + 5), sortedFrameData[0][indexOf0].sampleName.c_str());
			std::string timeTakenAsString = std::to_string(sortedFrameData[0][indexOf0].profile.timeTaken);
			ImGui::RenderText(ImVec2(minPos.x + 5, minPos.y + 15), timeTakenAsString.c_str());

			ImVec2 childStartPoint = ImVec2(canvas_p0.x + TotalBlockWidthSoFar, canvas_p0.y + layerHights);
			recusiveAddChildrenToDrawList(drawlist, sortedFrameData, childStartPoint, 1, sortedFrameData[0][indexOf0].profile.functionID, vecOfLayerWidths[0], layerHights);

			TotalBlockWidthSoFar += vecOfLayerWidths[0];
		}

		drawlist->PopClipRect();
	}


	ImGui::End();

	profiler->endProfile("EFlameGraphUpdate");
}
