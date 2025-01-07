#pragma once
#include <vector>

/// @brief This was originally inhereted by gameObject and used for the parent and child system,
///	it has been removed as casting gameobject->Inode to parent then Inode->gameObject is longwinded and slightly redundent 
class INode
{
protected:
	std::vector<INode*> childNode;
	INode* parentNode = nullptr;
	bool isRootNode = true;
	unsigned int Node_depth = 0;
	
public:
	virtual bool setParent(INode* parent) = 0;
	virtual INode* getParentINode() = 0;

	virtual bool AddChild(INode* child, int* id = NULL) = 0;	//returns a bool and the int* id will be changed to become the id of the child
	virtual bool RemoveChildWithID(int id) = 0;
	virtual bool RemoveChildWithRef(INode* nodeToRemove) = 0;
	virtual INode* getChild(int id) = 0;
	virtual bool  getIsRootNode() = 0;
	virtual bool setIsRootNode(bool newRootNodeValue) = 0;
	virtual int getDepth() = 0;
	virtual void setDepth(int newDepth) = 0;
	virtual void setParentB(INode* newParent) = 0;
	virtual void setChildB(INode* newChild) = 0;
	//virtual void setTransform(Transform* newTrans) = 0;	//my gameObject allready have transforms so this isnt nessacary
	//virtual Transform* getTransform() = 0;

	//not sure what these will be used for yet
	/*virtual void PreRender() = 0;	
	virtual void Render() = 0;
	virtual void PostRender() = 0;*/
};