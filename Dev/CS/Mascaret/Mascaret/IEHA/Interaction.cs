using System.Collections;
using System.Collections.Generic;
using System;
using System.IO;

namespace Mascaret
{
    public class Interaction
    {

        public List<ButtonActionInfo> interactions;

        public Interaction()
        {
            this.interactions = new List<ButtonActionInfo>();
        }

        public void addButtonAction(Button button, Action action, string target, bool pressed)
        {
            ButtonActionInfo bI = new ButtonActionInfo();
            bI.action = action;
            bI.button = button;
            bI.button.addCallbackOnButtonPressed(OnButtonPressed);
            bI.target = target;
            bI.pressed = pressed;

            interactions.Add(bI);
        }

        void OnButtonPressed(Button button, bool pressed)
        {


            foreach (ButtonActionInfo interaction in interactions)
            {
                if (interaction.pressed == pressed && interaction.button.name == button.name)
                {
                    Action action = interaction.action;
                    string target = interaction.target;


                    if (target == "broadcast")
                    {
                        //no target = broadcast. Just choose an arbitrary entity to execute
                        List<Entity> entities = MascaretApplication.Instance.getEnvironment().getEntities();
                        foreach (Entity entity in entities)
                        {
                            Action action2 = null;

                            if (action.GetType().ToString() == "Mascaret.SendSignalAction")
                            {
                                MascaretApplication.Instance.VRComponentFactory.Log("############################ Interaction");
                                Signal signalC = ((SendSignalAction)(action)).SignalClass;
                                InstanceSpecification signal = new InstanceSpecification(signalC.name, signalC);

                                action2 = new SendSignalAction();
                                ((SendSignalAction)(action2)).SignalClass = ((SendSignalAction)(action)).SignalClass;
                                ((SendSignalAction)(action2)).Target = new SendSignalTarget();
                                ((SendSignalAction)(action2)).Target.target = null;
                                ((SendSignalAction)(action2)).Signal = signal;
                            }
                            else if (action.GetType().ToString() == "Mascaret.CallOperationAction")
                            {
                                action2 = new CallOperationAction();
                                ((CallOperationAction)(action2)).Operation = ((CallOperationAction)(action)).Operation;
                                ((CallOperationAction)(action2)).Target = entity;
                            }
                            //if (agt)
                            //	BehaviorScheduler::getInstance()->executeBehavior(action2,agt,Parameters());
                            //else
                            BehaviorScheduler.Instance.executeBehavior(action2, entity, new Dictionary<string, ValueSpecification>(), false);
                            //_cbActionDone(action2);
                        }
                    }
                    else if (target == "designated")
                    {
                        InstanceSpecification entity = VRApplication.Instance.window.getSelectedEntity();


                        if (entity != null)
                        {

                            if (action.GetType().ToString() == "Mascaret.SendSignalAction")
                            {
                                Signal signalC = ((SendSignalAction)(action)).SignalClass;
                                InstanceSpecification signal = new InstanceSpecification(signalC.name, signalC);
                                ((SendSignalAction)(action)).Target = new SendSignalTarget();
                                ((SendSignalAction)(action)).Signal = signal;
                                ((SendSignalAction)(action)).Target.target = entity;

                            }
                            else if (action.GetType().ToString() == "Mascaret.CallOperationAction")
                            {
                                //cerr << "CALL OPERATION ..." << endl;
                                if (entity.Classifier.isA(((CallOperationAction)(action)).Operation.Classifier) == false)
                                {
                                    continue;
                                }
                                ((CallOperationAction)(action)).Target = entity;
                            }

                            //if (agt)
                            //	BehaviorScheduler::getInstance()->executeBehavior(action,agt,Parameters());
                            //else
                            BehaviorScheduler.Instance.executeBehavior(action, entity, new Dictionary<string, ValueSpecification>(), false);
                            //_cbActionDone(action);
                        }
                    }
                    else
                    {
                        if (action.GetType().ToString() == "Mascaret.CallProcedureAction")
                        {
                            List<Entity> entities = MascaretApplication.Instance.getEnvironment().getEntities();
                            Entity entity = entities[0];
                            Action action2 = null;
                            action2 = new CallProcedureAction();
                            ((CallProcedureAction)(action2)).Procedure = ((CallProcedureAction)(action)).Procedure;
                            ((CallProcedureAction)(action2)).OrganisationalEntity = ((CallProcedureAction)(action)).OrganisationalEntity;
                            BehaviorScheduler.Instance.executeBehavior(action2, entity, new Dictionary<string, ValueSpecification>(), false);
                        }
                    }
                }
            }
        }
    }
}
