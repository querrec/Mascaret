#include "VEHA/Behavior/StateMachine/Vertex.h"
#include "VEHA/Behavior/StateMachine/Transition.h"
#include "VEHA/Behavior/StateMachine/Transition.h"

namespace VEHA
{

Vertex::Vertex(const string& name)
:Element(name), running(false), _noninterupt(false)
{
}

Vertex::~Vertex()
{
}

}
