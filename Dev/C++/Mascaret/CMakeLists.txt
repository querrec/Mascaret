cmake_minimum_required(VERSION 2.8)
project(Mascaret)

set(SOURCE_FILES
  src/VEHA/Kernel/Element.cpp
  src/VEHA/Kernel/Class.cpp
  src/VEHA/Kernel/BehavioralFeature.cpp
  src/VEHA/Kernel/Property.cpp
  src/VEHA/Kernel/Classifier.cpp
  src/VEHA/Kernel/ValueSpecification.cpp
  src/VEHA/Kernel/DomainSpecification.cpp
  src/VEHA/Kernel/InstanceSpecification.cpp
  src/VEHA/Kernel/Slot.cpp
  src/VEHA/Kernel/SlotDerived.cpp
  src/VEHA/Kernel/Operation.cpp
  src/VEHA/Kernel/Constraint.cpp
  src/VEHA/Kernel/Expression.cpp
  src/VEHA/Kernel/InstanceValue.cpp
  src/VEHA/Kernel/OpaqueExpression.cpp
  src/VEHA/Kernel/Feature.cpp
  src/VEHA/Kernel/Parameter.cpp
  src/VEHA/Kernel/Comment.cpp
  src/VEHA/Kernel/Model.cpp
  src/VEHA/Kernel/Environment.cpp
  src/VEHA/Kernel/Package.cpp
  src/VEHA/Kernel/PrimitiveType.cpp
  src/VEHA/Kernel/LiteralSpecification.cpp
  src/VEHA/Kernel/LiteralReal.cpp
  src/VEHA/Kernel/LiteralInteger.cpp
  src/VEHA/Kernel/LiteralString.cpp
  src/VEHA/Kernel/LiteralBoolean.cpp
  src/VEHA/Entity/EntityClass.cpp
  src/VEHA/Entity/GeometryProperty.cpp
  src/VEHA/Entity/TopologicalProperty.cpp
  src/VEHA/Entity/ShapeProperty.cpp
  src/VEHA/Entity/Entity.cpp
  src/VEHA/Entity/PointSpecification.cpp
  src/VEHA/Entity/RotationVector.cpp
  src/VEHA/Entity/Vector3.cpp
  src/VEHA/Entity/Color.cpp
  src/VEHA/Entity/PointProperty.cpp
  src/VEHA/Entity/PathSpecification.cpp
  src/VEHA/Entity/PathProperty.cpp
  src/VEHA/Entity/PrimitiveGeometry.cpp
  src/VEHA/Entity/ShapeGeometry.cpp
  src/VEHA/Entity/SpecificGeometry.cpp
  src/VEHA/Entity/TopologicalSpecification.cpp
  src/VEHA/Entity/GeometrySpecification.cpp
  src/VEHA/Entity/ShapeSpecification.cpp
  src/VEHA/Entity/AnimationSpecification.cpp
  src/VEHA/Entity/AnimationProperty.cpp
  src/VEHA/Entity/SoundSpecification.cpp
  src/VEHA/Entity/SoundProperty.cpp
  src/VEHA/Entity/VirtualRealityComponentFactory.cpp
  src/VEHA/Entity/VirtualRealitySpecification.cpp
  src/VEHA/Entity/Zone.cpp
  src/VEHA/Behavior/StateMachine/CallEvent.cpp
  src/VEHA/Behavior/StateMachine/ChangeEvent.cpp
  src/VEHA/Behavior/StateMachine/CompositeState.cpp
  src/VEHA/Behavior/StateMachine/FinalState.cpp
  src/VEHA/Behavior/StateMachine/Guard.cpp
  src/VEHA/Behavior/StateMachine/PseudoState.cpp
  src/VEHA/Behavior/StateMachine/SignalEvent.cpp
  src/VEHA/Behavior/StateMachine/StateController.cpp
  src/VEHA/Behavior/StateMachine/State.cpp
  src/VEHA/Behavior/StateMachine/StateMachine.cpp
  src/VEHA/Behavior/StateMachine/StateMachineBehaviorExecution.cpp
  src/VEHA/Behavior/StateMachine/Region.cpp
  src/VEHA/Behavior/StateMachine/Vertex.cpp
  src/VEHA/Behavior/StateMachine/TimeEvent.cpp
  src/VEHA/Behavior/StateMachine/Transition.cpp
  src/VEHA/Behavior/Common/Behavior.cpp
  src/VEHA/Behavior/Common/Signal.cpp
  src/VEHA/Behavior/Common/BehaviorExecution.cpp
  src/VEHA/Behavior/Common/Event.cpp
  src/VEHA/Behavior/Common/EventOccurence.cpp
  src/VEHA/Behavior/Common/OpaqueBehavior.cpp
  #	src/VEHA/Behavior/Common/Operation.cpp
  src/VEHA/Behavior/Common/behaviorScheduler.cpp
  src/VEHA/Behavior/Common/Trigger.cpp
  src/VEHA/Behavior/Activity/ActionNode.cpp
  src/VEHA/Behavior/Activity/ActivityEdge.cpp
  src/VEHA/Behavior/Activity/ActivityFinalNode.cpp
  src/VEHA/Behavior/Activity/Activity.cpp
  src/VEHA/Behavior/Activity/ActivityExecution.cpp
  src/VEHA/Behavior/Activity/ActivityNode.cpp
  src/VEHA/Behavior/Activity/ActivityPartition.cpp
  src/VEHA/Behavior/Activity/ControlFlow.cpp
  src/VEHA/Behavior/Activity/ControlNode.cpp
  src/VEHA/Behavior/Activity/LoopNode.cpp
  src/VEHA/Behavior/Activity/LoopNodeBehavior.cpp
  src/VEHA/Behavior/Activity/LoopNodeBehaviorExecution.cpp
  src/VEHA/Behavior/Activity/DecisionNode.cpp
  src/VEHA/Behavior/Activity/FinalNode.cpp
  src/VEHA/Behavior/Activity/FlowFinalNode.cpp
  src/VEHA/Behavior/Activity/ForkNode.cpp
  src/VEHA/Behavior/Activity/ChoiceNode.cpp
  src/VEHA/Behavior/Activity/InitialNode.cpp
  src/VEHA/Behavior/Activity/JoinNode.cpp
  src/VEHA/Behavior/Activity/MergeNode.cpp
  src/VEHA/Behavior/Activity/ObjectFlow.cpp
  src/VEHA/Behavior/Activity/ObjectNode.cpp
  src/VEHA/Behavior/Activity/ActivityParameterNode.cpp
  src/VEHA/Behavior/Activity/ActivityBehaviorExecution.cpp
  src/VEHA/Behavior/BasicActions/Action.cpp
  src/VEHA/Behavior/BasicActions/AcceptEventAction.cpp
  src/VEHA/Behavior/BasicActions/AcceptEventBehaviorExecution.cpp
  src/VEHA/Behavior/BasicActions/AddStructuralFeatureValueAction.cpp
  src/VEHA/Behavior/BasicActions/AddStructuralFeatureValueBehaviorExecution.cpp
  src/VEHA/Behavior/BasicActions/CallOperationAction.cpp
  src/VEHA/Behavior/BasicActions/CallOperationBehaviorExecution.cpp
  src/VEHA/Behavior/BasicActions/CallBehaviorAction.cpp
  src/VEHA/Behavior/BasicActions/CallBehaviorBehaviorExecution.cpp
  src/VEHA/Behavior/BasicActions/GrowUpInstanceAction.cpp
  src/VEHA/Behavior/BasicActions/GrowUpInstanceBehaviorExecution.cpp
  src/VEHA/Behavior/BasicActions/SendSignalAction.cpp
  src/VEHA/Behavior/BasicActions/SendSignalBehaviorExecution.cpp
  src/VEHA/Behavior/BasicActions/CreateEntityAction.cpp
  src/VEHA/Behavior/BasicActions/CreateEntityBehaviorExecution.cpp
  src/VEHA/Behavior/BasicActions/RestoreEnvironmentAction.cpp
  src/VEHA/Behavior/BasicActions/RestoreEnvironmentBehaviorExecution.cpp
  src/VEHA/Behavior/BasicActions/SetInstanceInRedAction.cpp
  src/VEHA/Behavior/BasicActions/SetInstanceInRedBehaviorExecution.cpp
  src/VEHA/Behavior/BasicActions/SetInstanceTransparencyAction.cpp
  src/VEHA/Behavior/BasicActions/SetInstanceTransparencyBehaviorExecution.cpp
  src/VEHA/Behavior/BasicActions/PlayAnimationAction.cpp
  src/VEHA/Behavior/BasicActions/PlayAnimationBehaviorExecution.cpp
  src/VEHA/Behavior/BasicActions/StopAnimationAction.cpp
  src/VEHA/Behavior/BasicActions/StopAnimationBehaviorExecution.cpp
  src/VEHA/Behavior/BasicActions/ChangeViewPointAction.cpp
  src/VEHA/Behavior/BasicActions/ChangeViewPointBehaviorExecution.cpp
  src/VEHA/Behavior/BasicActions/OperationWidgetAction.cpp
  src/VEHA/Behavior/BasicActions/OperationWidgetBehaviorExecution.cpp
  src/VEHA/Behavior/BasicActions/DummyAction.cpp
  src/VEHA/Behavior/BasicActions/DummyBehaviorExecution.cpp
  src/VEHA/Behavior/BasicActions/SignalWidgetAction.cpp
  src/VEHA/Behavior/BasicActions/SignalWidgetBehaviorExecution.cpp
  src/VEHA/Behavior/BasicActions/OutputPin.cpp
  src/VEHA/Behavior/BasicActions/Pin.cpp
  src/VEHA/Behavior/BasicActions/InputPin.cpp
  src/VEHA/Behavior/BasicActions/ValuePin.cpp
  src/VEHA/Behavior/Time/TimeExpression.cpp
  #	src/VEHA/Entity/interface3D.cpp
  src/VEHA/Behavior/Common/pluginManager.cpp
  src/VEHA/Kernel/CollectionMethods.cpp
  src/VEHA/Kernel/expressionParser.cpp
  src/VEHA/Behavior/Common/OclQuery.cpp
  src/VEHA/Kernel/CallbackManager.cpp
  src/VEHA/Kernel/OCLSpecialMethods.cpp
  src/BEHAVE/Agent.cpp
  src/BEHAVE/AID.cpp
  src/BEHAVE/Mailbox.cpp
  src/BEHAVE/ACLMessage.cpp
  src/BEHAVE/fipaSLParser.cpp
  src/BEHAVE/AgentClass.cpp
  src/BEHAVE/KnowledgeBase.cpp
  src/BEHAVE/SimpleCommunicationBehavior.cpp
  src/BEHAVE/ProceduralBehavior.cpp
  src/BEHAVE/ActionListenerBehavior.cpp
  src/BEHAVE/AgentBehavior.cpp
  src/BEHAVE/AgentBehaviorExecution.cpp
  src/BEHAVE/SimpleBehaviorExecution.cpp
  src/BEHAVE/OneShotBehaviorExecution.cpp
  src/BEHAVE/CyclicBehaviorExecution.cpp
  src/BEHAVE/AgentPlateform.cpp
  src/BEHAVE/OrganisationalStructure.cpp
  src/BEHAVE/OrganisationalEntity.cpp
  src/BEHAVE/Ressource.cpp
  src/BEHAVE/Role.cpp
  src/BEHAVE/RoleClass.cpp
  src/BEHAVE/RoleAssignement.cpp
  src/BEHAVE/RessourceAssignement.cpp
  src/BEHAVE/Procedure.cpp
  src/BEHAVE/ProcedureExecution.cpp
  src/HAVE/EmbodiedAgent.cpp
  src/BEHAVE/HttpCommunicationManager.cpp
  src/HAVE/Body.cpp
  src/HAVE/BodyClass.cpp
  src/HAVE/VirtualHumanClass.cpp
  src/HAVE/VirtualHuman.cpp
  src/Tools/Loaders/XmlParser.cpp
  src/Tools/Loaders/XMIParser.cpp
  src/Tools/Loaders/ModelLoader.cpp
  src/Tools/Loaders/InstanceLoader.cpp
  src/Tools/Loaders/ParserHelper.cpp
  src/MascaretApplication.cpp 
  #src/Tools/Loaders/InstanceWriter.cpp
  src/Tools/md5.cpp
  src/Tools/Loaders/ModelLoader2.cpp
  src/Tools/Loaders/AbstractModelLoader.cpp
  src/Tools/Loaders/XMI11Loader.cpp
  #src/Tools/Loaders/XMI20Writer.cpp
  src/IEHA/Visualisation/Scene.cpp
  src/IEHA/Visualisation/Camera.cpp
  src/IEHA/VRApplication.cpp
  src/IEHA/Visualisation/Renderer.cpp
  src/IEHA/Visualisation/RenderToTexture.cpp
  src/IEHA/Visualisation/Window3D.cpp
  src/IEHA/Interaction/Peripheric.cpp
  src/IEHA/Interaction/PaddlePeripheric.cpp
  src/IEHA/Interaction/Axis.cpp
  src/IEHA/Interaction/Button.cpp
  src/IEHA/Interaction/ButtonAxis.cpp
  src/IEHA/Interaction/CartesianBasis.cpp
  src/IEHA/Interaction/Interaction.cpp
  src/IEHA/Interaction/Navigation.cpp
  src/SysML/Block.cpp

  src/Tools/Scenario/Scenario.cpp
  src/Tools/Scenario/ScenarioEvent.cpp
  src/Tools/Scenario/ScenarioScheduler.cpp

  #backup module
  src/BackupUtils/BackupList.cpp
  src/BackupUtils/IBackupEnabled.cpp
  src/BackupUtils/Backup.cpp
  src/BackupUtils/ProcedureExecutionBackup.cpp
  src/BackupUtils/SlotBackup.cpp
)

set(HTTP_SERVER_SOURCES_FILES
  src/Tools/Network/HttpServer/httpConnection.cpp
  src/Tools/Network/HttpServer/httpRequest.cpp
  src/Tools/Network/HttpServer/httpResponse.cpp
  src/Tools/Network/HttpServer/httpServer.cpp
  src/Tools/Network/HttpServer/httpSession.cpp
  src/Tools/Network/HttpServer/tcpConnection.cpp
  src/Tools/Network/HttpServer/tcpServer.cpp
  src/Tools/Network/Servlets/ManageACLMessageServlet.cpp
  src/Tools/Network/Servlets/ManageActionServlet.cpp
  src/Tools/Network/Servlets/ManageAgentServlet.cpp
  src/Tools/Network/Servlets/ManageAgentKnowledgeBaseServlet.cpp
  src/Tools/Network/Servlets/ManageAgentKBHeadServlet.cpp
  src/Tools/Network/Servlets/ManageAgentsServlet.cpp
  src/Tools/Network/Servlets/ManageApplicationServlet.cpp
  src/Tools/Network/Servlets/ManageCenterViewServlet.cpp
  src/Tools/Network/Servlets/ManageChangeGeometryServlet.cpp
  src/Tools/Network/Servlets/ManageClassServlet.cpp
  src/Tools/Network/Servlets/ManageReloadClassServlet.cpp
  src/Tools/Network/Servlets/ManageConnectServlet.cpp
  src/Tools/Network/Servlets/ManageCreateEntityServlet.cpp
  src/Tools/Network/Servlets/ManageCreateMessageServlet.cpp
  src/Tools/Network/Servlets/ManageCreateUserMessageServlet.cpp
  src/Tools/Network/Servlets/ManageEntityServlet.cpp
  src/Tools/Network/Servlets/ManageEntityJSONServlet.cpp
  src/Tools/Network/Servlets/ManageExecuteQueryServlet.cpp
  src/Tools/Network/Servlets/ManageFIPASLServlet.cpp
  src/Tools/Network/Servlets/ManageFIPASLTestServlet.cpp
  src/Tools/Network/Servlets/ManageHeadServlet.cpp
  src/Tools/Network/Servlets/ManageIndexServlet.cpp
  src/Tools/Network/Servlets/ManageInstancesServlet.cpp
  src/Tools/Network/Servlets/ManageLaunchOperationServlet.cpp
  src/Tools/Network/Servlets/ManageMessagesEnvoyesServlet.cpp
  src/Tools/Network/Servlets/ManageMessagesRecusServlet.cpp
  src/Tools/Network/Servlets/ManageMessagesServlet.cpp
  src/Tools/Network/Servlets/ManageModelServlet.cpp
  src/Tools/Network/Servlets/ManageOperationServlet.cpp
  src/Tools/Network/Servlets/ManageOrganisationalEntityServlet.cpp
  src/Tools/Network/Servlets/ManageOrganisationalStructureServlet.cpp
  src/Tools/Network/Servlets/ManageOrganisationsServlet.cpp
  src/Tools/Network/Servlets/ManageProcedureServlet.cpp
  src/Tools/Network/Servlets/ManageLaunchProcedureServlet.cpp
  src/Tools/Network/Servlets/ManageQueryServlet.cpp
  src/Tools/Network/Servlets/ManageRoleClassServlet.cpp
  src/Tools/Network/Servlets/ManageSendMessageServlet.cpp
  src/Tools/Network/Servlets/ManageSendUserMessageServlet.cpp
  src/Tools/Network/Servlets/ManageSetRedServlet.cpp
  src/Tools/Network/Servlets/ManageSignalServlet.cpp
  src/Tools/Network/Servlets/ManageXOCLServlet.cpp
  src/Tools/Network/Servlets/ManageAttributeServlet.cpp
  #src/Tools/Network/Servlets/ManageXOCLHelperServlet.cpp
  #src/Tools/Network/Servlets/ManageXOCLHelperRemoveServlet.cpp
  src/Tools/Network/Servlets/ManageScenarioServlet.cpp
  src/Tools/Network/Servlets/ManageChangeAttributeServlet.cpp
  src/Tools/Network/Servlets/ManageSetAttributeValueServlet.cpp
  src/Tools/Network/Servlets/ManageSpeedServlet.cpp
)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-W -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-long-long)
endif(CMAKE_COMPILER_IS_GNUCXX)

add_definitions(-D_VEHA_LIB -D_HTTPSERVER_LIB -DNOPYTHON)
include_directories(./include/ ./include/Tools/Network)

find_package(LibXml2 REQUIRED)
include_directories(SYSTEM ${LIBXML2_INCLUDE_DIR})
link_directories(${LIBXML2_LIBRARY_DIR})

set(BOOST_COMPONENTS thread date_time system filesystem regex signals)
include(FindBoost)
find_package(
	Boost
	1.36.0
	COMPONENTS ${BOOST_COMPONENTS} REQUIRED
)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_library(httpServer STATIC ${HTTP_SERVER_SOURCES_FILES})
add_library(Mascaret SHARED ${SOURCE_FILES})

target_link_libraries(Mascaret httpServer)
target_link_libraries(Mascaret ${Boost_LIBRARIES} ${LIBXML2_LIBRARIES})

if(WIN32 AND CMAKE_CROSSCOMPILING)
    set_target_properties(Mascaret PROPERTIES LINK_FLAGS "-Wl,--enable-auto-import")
    target_link_libraries(Mascaret ws2_32 mswsock)
endif()

# Linkage statique de libgcc et libstdc++ en cas de crosscompilation
if(WIN32 AND CMAKE_CROSSCOMPILING AND CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(Mascaret PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
endif()
