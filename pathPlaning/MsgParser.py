import json


msgInstantControl = {
        "msgType":0,
        "msgID":0,
        "v":0,
        "r":0}


msgTrack = {
        "msgType": 1,
        "msgID":0,
        "track": { "t":[],
                    "d":[],
                    "r":[]
                }}

msgResponse = {
        "msgType": 2,
        "msgID":0,
        "responseForID":0}

msgID = 0


def getInstantControlMSG(d, r):
    global msgInstantControl
    msg = setID(msgInstantControl)
    msg["d"] = d
    msg["r"] = r
    return json.dumps(msgInstantControl)

def getTrackMSG(t, d, r):
    global msgTrack
    msg = setID(msgTrack)
    msg["track"]["t"] = t
    msg["track"]["d"] = d
    msg["track"]["r"] = r
    return json.dumps(msg)

def getResponseMSGForID(responseID):
    global msgInstantControl
    msg = setID(msgInstantControl)
    msg["responseForID"] = responseID
    return json.dumps(msgInstantControl)

def getStopMSG():
    return getInstantControlMSG(0, 0)

def setID(msg):
    global msgID
    msg["msgID"] = msgID
    msgID += 1
    return msg
