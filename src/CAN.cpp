#include "../include/CAN.h"

// Create an object dictionary to store the messages
CAN::ObjectDictionary CAN::m_objectDict;

// Create a message flag map to track new messages
CAN::MessageFlag CAN::m_messageFlag; 

CAN::CAN()
{  
  // Set stop message to 0
  CANMessage stopMessage;
  stopMessage.id = CAN::E_STOP;
  stopMessage.len = MSG_LENGTH;
  for(int i = 0; i < MSG_LENGTH; i++)
  {
    stopMessage.data[i] = 0;
  }

  m_objectDict[CAN::E_STOP] = stopMessage;
}

void CAN::startCAN()
{
  // const ACANPrimaryFilter primaryFilter(&CAN::CANSniff);
  // Set the settings to the CAN
  ACAN_T4_Settings settings (CAN_BAUDRATE) ;
  settings.mTxPinIsOpenCollector = true ;
  settings.mRxPinConfiguration = ACAN_T4_Settings::NO_PULLUP_NO_PULLDOWN ;

  // // Start the CAN bus based on ActiveCAN
  if(CAN::CAN_MODE::CAN1 == ActiveCAN)
  {
    ACAN_T4::can1.begin (settings);//, &primaryFilter, 1) ;
  }
  else if(CAN::CAN_MODE::CAN2 == ActiveCAN)
  {
    ACAN_T4::can2.begin (settings);//, &primaryFilter, 1) ;
  }
  else if(CAN::CAN_MODE::CAN3 == ActiveCAN)
  {
    ACAN_T4::can3.begin (settings);//, &primaryFilter, 1) ;
  }
  #if ENABLE_SERIAL
    Serial.println("CAN StartUp");
  #endif
}

// Function to be called when a message is recieved
void CAN::CANSniff(const CANMessage &msg)
{
  Message_ID id = static_cast<Message_ID>(msg.id);

  // Check if the message will need to be filtered due to an E-Stop
  if(IsEStop(msg))
  {
    m_objectDict.at(CAN::E_STOP).data[0] = 1;
    return;
  }
  else
  {
    m_objectDict.at(CAN::E_STOP).data[0] = 0;
  }

  // Check if the ID exists in the m_objectDict map
  if (m_objectDict.find(id) == m_objectDict.end()) {
    // This is the first time the message with this ID is being received
    m_messageFlag[id] = true;
  }
  // Check if the message has changed and update message flags
  else
  {
    bool newMessage = false;
    const auto &existingMessage = m_objectDict[id];

    for(unsigned int i=0; i<sizeof(msg.data); i++)
    {
      if(msg.data[i] != existingMessage.data[i])
      {
        newMessage = true;
        break;
      }
    }
    m_messageFlag[id] = newMessage;
  }

  m_objectDict[id] = msg;

  #if ENABLE_SERIAL
  Serial.print("Message Received");
  Serial.println(msg.id);
  #endif
}

void CAN::readMsgBuffer(void)
{
  // the current msg
  CANMessage message;

  // get the message from the hardware.
  if(CAN::CAN_MODE::CAN1 == ActiveCAN)
  {
    // if doesn't have message then end
    if(!ACAN_T4::can1.receive(message))
    {
      return;
    }
  }
  else if(CAN::CAN_MODE::CAN2 == ActiveCAN)
  {
    // if doesn't have message then end
    if(!ACAN_T4::can2.receive(message))
    {
      return;
    }
  }
  else if(CAN::CAN_MODE::CAN3 == ActiveCAN)
  {
    // if doesn't have message then end
    if(!ACAN_T4::can3.receive(message))
    {
      return;
    }
  }

  CANSniff(message);
}


// Send a message to the CAN bus
void CAN::sendMessage( CAN_MB mailBox, Message_ID id, uint8_t message[MSG_LENGTH])
{
  // Create a message
  CANMessage msg;

  // Set the message ID to 0x123
  msg.id = id;
  
  // Set the message length to 8
  msg.len = MSG_LENGTH;

  // Set the message buffer to the m_message buffer
  for(int i = 0; i < MSG_LENGTH; i++)
  {
    msg.data[i] = message[i];
  }

  // Add the message to the object dictionary
  m_objectDict[static_cast<Message_ID>(msg.id)] = msg;

  // Send the message
  if(CAN::CAN_MODE::CAN1 == ActiveCAN)
  {
    ACAN_T4::can1.tryToSend (msg);
  }
  else if(CAN::CAN_MODE::CAN2 == ActiveCAN)
  {
    ACAN_T4::can2.tryToSend (msg);
  }
  else if(CAN::CAN_MODE::CAN3 == ActiveCAN)
  {
    ACAN_T4::can3.tryToSend (msg);
  }
}

// get message out of object dictionary, unpacked. For some unpackage index will matter otherwise not important
int CAN::getUnpackedMessage(Message_ID id, int index = 0)
{
  switch(id)
  {
    case Message_ID::E_STOP:
    return m_objectDict.at(CAN::Message_ID::E_STOP).data[0];
    break;

    case Message_ID::TARGET_RPM:
    return m_objectDict.at(CAN::Message_ID::TARGET_RPM).data[index];
    break;

    case Message_ID::CURRENT_RPM:
    return m_objectDict.at(CAN::Message_ID::CURRENT_RPM).data[index];
    break;

    default:
    return 0;
    break;

  }
}

// checks if there has been a new msg on the id
bool CAN::isNewMessage(Message_ID id)
{
    auto it = m_messageFlag.find(id);
    if (it != m_messageFlag.end()) {
        return it->second;
    } else {
        // No message has been received yet at this ID
        return false;
    }
}

bool CAN::IsEStop(const CANMessage &msg)
{
  //if the message E_STOP is off in object dictionary 
  if(m_objectDict.at(CAN::E_STOP).id == 0)
  {
    // if the message is an E-Stop message and the E-Stop is non active turn on the E-Stop
    if(msg.id == CAN::E_STOP && msg.data[0] == 1)
    {
      // for each message in the object dictionary, set the message flag to true and clear the message buffer
      for(const auto &message : m_objectDict)
      {
        if(message.first != CAN::E_STOP)
        {
          m_messageFlag[message.first] = true;
          for(int i = 0; i < MSG_LENGTH; i++)
          {
            m_objectDict[message.first].data[i] = 0;
          }
        }
      }
      // filter out the message from canSniff
      return true;
    }

    // normal state, do not filter the message
    return false;
  }
  else
  {
    // if the message is an E-Stop message that is and the E-Stop is active turn off the E-Stop
    if(msg.id == CAN::E_STOP && msg.data[0] == 0)
    {
      // do not filter the message. E-Stop will turn off naturally with canSniff
      return false;
    }
    // E_STOP is active, filter all messages
    return true;
  }
}
