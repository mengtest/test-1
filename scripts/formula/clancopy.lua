local BufferValue = {
    [0x01] = {
        0.0000,
        0.0060, 0.0140, 0.0240, 0.0360, 0.0500, 0.0557, 0.0610, 0.0660, 0.0707, 0.0750, 
        0.0790, 0.0827, 0.0860, 0.0890, 0.0917, 0.0940, 0.0960, 0.0977, 0.0990, 0.1000, 
    },
    [0x11] = {
        0.0000,
        0.0060, 0.0140, 0.0240, 0.0360, 0.0500, 0.0557, 0.0610, 0.0660, 0.0707, 0.0750, 
        0.0790, 0.0827, 0.0860, 0.0890, 0.0917, 0.0940, 0.0960, 0.0977, 0.0990, 0.1000, 
    },
    [0x21] = {
        0.0000,
        0.0060, 0.0140, 0.0240, 0.0360, 0.0500, 0.0557, 0.0610, 0.0660, 0.0707, 0.0750, 
        0.0790, 0.0827, 0.0860, 0.0890, 0.0917, 0.0940, 0.0960, 0.0977, 0.0990, 0.1000, 
    },
    [0x31] = {
        0.0000,
        0.0060, 0.0140, 0.0240, 0.0360, 0.0500, 0.0557, 0.0610, 0.0660, 0.0707, 0.0750, 
        0.0790, 0.0827, 0.0860, 0.0890, 0.0917, 0.0940, 0.0960, 0.0977, 0.0990, 0.1000, 
    },
    [0xff] = {
        0.0000,
        0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 
        0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 
    },
}

function LoadClanCopySpotBufferType()
    return {0x01, 0x11, 0x21, 0x31, 0xff}
end
function LoadClanCopySpotBufferValue(type)
    return BufferValue[type]
end

