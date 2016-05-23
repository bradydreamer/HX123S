TAG_TYPE tagTypeArray[] =

        {

                {0x9F01, TAG_TYPE_N},
                {0x9F02, TAG_TYPE_N},
                {0x9F03, TAG_TYPE_N},

                {0x9F42, TAG_TYPE_N},
                {0x9F44, TAG_TYPE_N},
                {0x5F25, TAG_TYPE_N},

                {0x5F24, TAG_TYPE_N},
                {0x5F34, TAG_TYPE_N},
                {0x9F3B, TAG_TYPE_N},

                {0x9F43, TAG_TYPE_N},
                {0x9F11, TAG_TYPE_N},
                {0x5F28, TAG_TYPE_N},

                {0x42,   TAG_TYPE_N},
                {0x9F15, TAG_TYPE_N},
                {0x9F39, TAG_TYPE_N},

                {0x5F30, TAG_TYPE_N},
                {0x9F1A, TAG_TYPE_N},
                {0x9F35, TAG_TYPE_N},

                {0x5F2A, TAG_TYPE_N},
                {0x5F36, TAG_TYPE_N},
                {0x9A,   TAG_TYPE_N},

                {0x9F3C, TAG_TYPE_N},
                {0x9F3D, TAG_TYPE_N},
                {0x9F41, TAG_TYPE_N},

                {0x9F21, TAG_TYPE_N},
                {0x9C,   TAG_TYPE_N},

                {0x5A,   TAG_TYPE_CN},
                {0x9F20, TAG_TYPE_CN}

        };



//------------------------------------------------------------------------------------------

void InitTagList(TagList *pTagList) {
    memset(pTagList, 0, sizeof(TagList));
}



//------------------------------------------------------------------------------------------

void FreeTagList(TagList *pTagList) {

    u32 index;


    for (index = 0; index < pTagList->ItemCount; index++) {

        sdkFreeMem(pTagList->tagItem[index].Value);

    }


    memset(pTagList, 0, sizeof(TagList));

}



//------------------------------------------------------------------------------------------

void appendTagList(TagList *sourceTagList, TagList *descTagList) {

    u32 index;


    for (index = 0; index < sourceTagList->ItemCount; index++) {

        SetTagValue(sourceTagList->tagItem[index].Tag,

                    sourceTagList->tagItem[index].Value,

                    sourceTagList->tagItem[index].Length, descTagList);

    }

}



//------------------------------------------------------------------------------------------

u32 GetTagCount(TagList *pTagList) {

    return pTagList->ItemCount;

}



//------------------------------------------------------------------------------------------

s32 GetTagIndex(TagList *pTagList, u16 tag) {
    u32 i;

    for (i = 0; i < pTagList->ItemCount; i++) {

        if (pTagList->tagItem[i].Tag == tag) {
            return i;
        }
    }

    return (-1);
}



//------------------------------------------------------------------------------------------

bool TagIsExisted(TagList *pTagList, u16 tag) {

    if (GetTagIndex(pTagList, tag) < 0) {
        return FALSE;
    }
    else {
        return TRUE;
    }
}



//------------------------------------------------------------------------------------------

bool TagDataIsMissing(TagList *pTagList, u16 tag) {

    if (!TagIsExisted(pTagList, tag)) {

        return TRUE;

    }


    if (GetTagValueSize(pTagList, tag) == 0) {

        return TRUE;

    }


    return FALSE;

}



//------------------------------------------------------------------------------------------

bool TagIsDuplicate(TagList *pTagList, u16 tag) {

    u32 index;


    for (index = 0; index < pTagList->duplicateItemCount; index++) {

        if (tag == pTagList->duplicateItem[index]) {

            return TRUE;

        }

    }


    return FALSE;

}



//------------------------------------------------------------------------------------------

bool TagIsTemplate(u16 tag) {


    if ((tag & 0xFF00)) {

        if ((tag & 0x2000)) {

            return TRUE;

        }

    }

    else {

        if (tag & 0x0020) {

            return TRUE;

        }

    }


    return FALSE;

}



//------------------------------------------------------------------------------------------

u32 SetTagItem(u16 tag, u8 *pValue, u32 length, TagItem *tagItem) {

    tagItem->Value = (u8 *) sdkGetMem(length);


    if (NULL == tagItem->Value) {

        return SDK_ERR;

    }


    tagItem->Tag = tag;

    tagItem->Length = length;

    memcpy(tagItem->Value, pValue, length);


    return SDK_OK;

}



//------------------------------------------------------------------------------------------

u32 AppendTagValue(u16 tag, u8 *pValue, u32 length, TagList *pTagList) {

    if (pTagList->ItemCount >= MAX_TAG_LIST_SIZE) {

        return SDK_ERR;

    }

/*

	if (length==0)

	{

		return SDK_OK;

	}

*/

    if (SDK_OK != SetTagItem(tag, pValue, length, &pTagList->tagItem[pTagList->ItemCount])) {

        return SDK_ERR;

    }


    (pTagList->ItemCount)++;


    return SDK_OK;

}



//------------------------------------------------------------------------------------------

u32 SetTagValue(u16 tag, u8 *pValue, u32 length, TagList *pTagList) {

    printf("pTagList->ItemCount = %d\n", pTagList->ItemCount);
    if (pTagList->ItemCount >= MAX_TAG_LIST_SIZE) {
        printf("pTagList->ItemCount = %d\n", pTagList->ItemCount);
        return SDK_ERR;

    }

/*

	if (length==0)

	{

		return SDK_OK;

	}

*/

    if (TagIsExisted(pTagList, tag)) {

        return SetTagItem(tag, pValue, length, &pTagList->tagItem[GetTagIndex(pTagList, tag)]);

    }

    else {

        return AppendTagValue(tag, pValue, length, pTagList);

    }

}



//------------------------------------------------------------------------------------------

u8 *GetTagValue(TagList *pTagList, u16 tag) {

    if (TagIsExisted(pTagList, tag)) {

        return pTagList->tagItem[GetTagIndex(pTagList, tag)].Value;

    }

    else {

        return NULL;

    }

}



//------------------------------------------------------------------------------------------

u32 GetTagValueSize(TagList *pTagList, u16 tag) {

    if (TagIsExisted(pTagList, tag)) {

        return pTagList->tagItem[GetTagIndex(pTagList, tag)].Length;

    }

    else {

        return 0;

    }

}



//------------------------------------------------------------------------------------------

u32 ParseTlvTagInfo(u8 *pTagStartPos, u16 size, u16 *pTag, u16 *pTagSize) {

    if (0 == size) {

        return SDK_ERR;

    }


    if ((*pTagStartPos & 0x1F) == 0x1F) {

        if (*(pTagStartPos + 1) & 0x80) {

            return SDK_ERR;

        }


        *pTag = (u16)(((*pTagStartPos) << 8) + *(pTagStartPos + 1));

        *pTagSize = 2;

    }

    else {

        *pTag = *pTagStartPos;

        *pTagSize = 1;

    }


    if (*pTagSize >= size) {

        return SDK_ERR;

    }


    return SDK_OK;

}



//------------------------------------------------------------------------------------------

u32 ParseTlvLengthInfo(u8 *pLenStartPos, u16 size,

                       u16 *pLength, u16 *pLengthSize) {

    u8 i;


    if (0 == size) {

        return SDK_ERR;

    }


    if (*pLenStartPos & 0x80) {

        if ((*pLenStartPos & 0x7F) > 2) {

            return SDK_ERR;

        }


        *pLengthSize = (u16)(1 + (*pLenStartPos & 0x7F));


        *pLength = 0;

        pLenStartPos += 1;

        for (i = 0; i < (*pLengthSize) - 1; i++) {

            *pLength <<= 8;

            *pLength = (u16)(*pLength + *pLenStartPos);

            pLenStartPos++;

        }

    }

    else {

        *pLength = *pLenStartPos;

        *pLengthSize = 1;

    }


    if (*pLengthSize > size) {

        return SDK_ERR;

    }


    return SDK_OK;

}



//------------------------------------------------------------------------------------------

u32 ParseTlvInfo(u8 *pTagStartPos, u16 size, u16 *pTag, u16 *pTagSize,

                 u16 *pLength, u16 *pLengthSize) {

    u8 *CurPos = (u8 *) pTagStartPos;

    u8 *EndPos = (u8 * )(pTagStartPos + size);


    if (SDK_OK != ParseTlvTagInfo(CurPos, (u16)(EndPos - CurPos), pTag, pTagSize)) {

        return SDK_ERR;

    }


    CurPos += *pTagSize;


    if (SDK_OK != ParseTlvLengthInfo(CurPos, (u16)(EndPos - CurPos), pLength, pLengthSize)) {
        //for 4.2c
        Trace("Mai", "ParseTlvLengthInfo error\n");
        return SDK_ERR;

    }


    return SDK_OK;

}



//------------------------------------------------------------------------------------------

u32 BuildTagList(u8 *buf, u32 size, TagList *pTagList) {

    return BuildTagListInMode(buf, size, 0, pTagList);

}



//------------------------------------------------------------------------------------------

u32 BuildTagListInMode(u8 *buf, u32 size, u32 mode, TagList *pTagList) {
    u16 Tag;
    u16 TagSize;
    u16 lengthSize;
    u16 Length;
    u8 *CurPos, *EndPos;

    if ((buf == NULL) || (size == 0)) {
        return SDK_OK;
    }

    //printf("TLV vaule:\n");
    TraceHex("Mai", "Tlv", buf, size);

    CurPos = buf;
    EndPos = (buf + size);


    //Trace("Mai", "00000000000\n");
    while (CurPos < EndPos) {
        //Trace("Mai", "1111111111111111\n");
        if ((*CurPos == 0x00) || (*CurPos == 0xFF)) {
            Trace("Mai", "(*CurPos==0x00)||(*CurPos==0xFF)\n");
            CurPos++;
            continue;
        }

        //Trace("Mai", "222222222222222222\n");
        if (SDK_OK !=
            ParseTlvInfo(CurPos, (u16)(EndPos - CurPos), &Tag, &TagSize, &Length, &lengthSize)) {
            Trace("Mai", "ParseTlvInfo error\n");
            return SDK_ERR;

        }

        CurPos += TagSize + lengthSize;

        //Trace("Mai", "222222221111111111222222\n");
        if (TagIsExisted(pTagList, Tag)) {
            pTagList->duplicateItem[pTagList->duplicateItemCount] = Tag;
            (pTagList->duplicateItemCount)++;
        }

        //Trace("Mai", "33333333333333\n");
        if (TagIsTemplate(Tag)) {

            if (mode & TAGMODE_RESERVE_ALL_ITEM) {
                if (mode & TAGMODE_DUPLICATE_ITEM_ALLOWED) {
                    if (SDK_OK != AppendTagValue(Tag, CurPos, Length, pTagList)) {
                        Trace("Mai", "AppendTagValue error\n");
                        return SDK_ERR;
                    }
                }
                else {
                    if (SDK_OK != SetTagValue(Tag, CurPos, Length, pTagList)) {
                        Trace("Mai", "SetTagValue error\n");
                        return SDK_ERR;
                    }
                }

            }

            if (SDK_OK != BuildTagListInMode(CurPos, Length, mode, pTagList)) {

                Trace("Mai", "BuildTagListInMode error\n");
                return SDK_ERR;

            }

        }

        else {

            if (mode & TAGMODE_DUPLICATE_ITEM_ALLOWED) {

                if (SDK_OK != AppendTagValue(Tag, CurPos, Length, pTagList)) {
                    Trace("Mai", "Not TEMP: BuildTagListInMode error\n");
                    return SDK_ERR;

                }

            }

            else {

                //printf("Tag = [%04x], Length = %d\n", Tag, Length);
                //printByteArray(CurPos, Length);
                if (SDK_OK != SetTagValue(Tag, CurPos, Length, pTagList)) {
                    Trace("Mai", "Not TEMP: SetTagValue error\n");

                    return SDK_ERR;

                }

            }

        }

        //Trace("Mai", "4444444444444444444\n");

        CurPos += Length;

    }


    if (CurPos != EndPos) {
        Trace("Mai", "CurPos != EndPos\n");
        if (CurPos > EndPos) {
            Trace("Mai", "CurPos > EndPos\n");
        }
        return SDK_ERR;

    }

    else {

        return SDK_OK;

    }

}



//------------------------------------------------------------------------------------------

u32 GetTagType(u16 tag) {

    u32 index, tagCount;


    tagCount = sizeof(tagTypeArray) / sizeof(TAG_TYPE);


    for (index = 0; index < tagCount; index++) {

        if (tag == tagTypeArray[index].Tag) {

            return tagTypeArray[index].Type;

        }

    }


    return TAG_TYPE_OTHER;

}



//------------------------------------------------------------------------------------------

void FillDataField(TagList *tagList, u16 tag, u16 ValueLength, u8 *resultStream) {

    u16 Length;

    u8 *pValue;

    u8 zeroBuffer[256] = {0};


    memset(resultStream, 0, ValueLength);

    if (TagIsTemplate(tag)) {

        return;

    }

    pValue = GetTagValue(tagList, tag);

    if (pValue != NULL) {

        Length = GetTagValueSize(tagList, tag);

    }

    else {

        pValue = zeroBuffer;

        Length = 256;

    }


    if (ValueLength < Length) {

        if (GetTagType(tag) == TAG_TYPE_N) {

            memcpy(resultStream, pValue + Length - ValueLength, ValueLength);

        }

        else {

            memcpy(resultStream, pValue, ValueLength);

        }

    }

    else if (ValueLength == Length) {

        memcpy(resultStream, pValue, ValueLength);

    }

    else {

        if (GetTagType(tag) == TAG_TYPE_N) {

            memset(resultStream, 0, ValueLength);

            memcpy(resultStream + ValueLength - Length,

                   pValue, Length);

        }

        else if (GetTagType(tag) == TAG_TYPE_CN) {

            memset(resultStream, 0xFF, ValueLength);

            memcpy(resultStream, pValue, Length);

        }

        else {

            memset(resultStream, 0, ValueLength);

            memcpy(resultStream, pValue, Length);

        }

    }

}



//------------------------------------------------------------------------------------------

bool tagIsInDOL(TagList *tagList, u16 dolType, u16 targetTag) {

    u16 tag;

    u16 tagSize;


    u16 length, lengthSize;

    u8 *curPos, *endPos;


    curPos = GetTagValue(tagList, dolType);

    if (curPos == NULL) {

        return FALSE;

    }


    endPos = curPos + GetTagValueSize(tagList, dolType);


    while (curPos < endPos) {

        if (SDK_OK != ParseTlvInfo(curPos, (u16)(endPos - curPos), &tag, &tagSize,

                                   &length, &lengthSize)) {

            return FALSE;

        }


        if (targetTag == tag) {

            return TRUE;

        }


        curPos += tagSize + lengthSize;

    }


    return FALSE;

}



//------------------------------------------------------------------------------------------

u32 BuildDOLToStream(TagList *tagList, u16 dolType,

                     u8 *resultStream, u8 *streamLength) {

    u16 tag;

    u16 tagSize;


    u16 length, lengthSize;

    u8 *curPos, *endPos;


    curPos = GetTagValue(tagList, dolType);

    if (curPos == NULL) {

        return SDK_ERR;

    }


    endPos = curPos + GetTagValueSize(tagList, dolType);


    *streamLength = 0;

    while (curPos < endPos) {

        if (SDK_OK != ParseTlvInfo(curPos, (u16)(endPos - curPos), &tag, &tagSize,

                                   &length, &lengthSize)) {

            return SDK_ERR;

        }


        curPos += tagSize + lengthSize;

        FillDataField(tagList, tag, length, resultStream + *streamLength);


        (*streamLength) += length;

    }


    if (curPos != endPos) {

        return SDK_ERR;

    }

    else {

        return SDK_OK;

    }

}

//Jason added on 2012.04.17, start
u32 ParseDOLStreamToTagList(TagList *tagList, u16 dolType,
                            u8 *Stream, u8 streamLength) {
    u16 tag;
    u16 tagSize;
    u16 length, lengthSize;
    u8 *curPos, *endPos;
    u8 streamOffset = 0;

    curPos = GetTagValue(tagList, dolType);
    if (curPos == NULL) {
        printf("1111\n");
        return SDK_ERR;
    }

    endPos = curPos + GetTagValueSize(tagList, dolType);
    while (curPos < endPos) {
        if (SDK_OK != ParseTlvInfo(curPos, (u16)(endPos - curPos), &tag, &tagSize,
                                   &length, &lengthSize)) {
            printf("2222\n");
            return SDK_ERR;
        }

        curPos += tagSize + lengthSize;
        //FillDataField(tagList, tag, length, resultStream + *streamLength);
        SetTagValue(tag, Stream + streamOffset, length, tagList);

        streamOffset += length;
    }

    if (curPos != endPos) {
        printf("3333\n");
        return SDK_ERR;
    }
    else if (streamOffset != streamLength) {
        printf("4444\n");
        return SDK_ERR;
    }

    return SDK_OK;

}
//End

//------------------------------------------------------------------------------------------

void PrintOutTagList(TagList *pTagList, char *promoptMsg) {

    u16 i;


    if (promoptMsg != NULL) {

        printf("%s\n", promoptMsg);

    }

    printf("count=%d\n", pTagList->ItemCount);

    for (i = 0; i < pTagList->ItemCount; i++) {

        printf("tag=%X length=%d\n", pTagList->tagItem[i].Tag, pTagList->tagItem[i].Length);

        printByteArray(pTagList->tagItem[i].Value, pTagList->tagItem[i].Length);

    }

}



//------------------------------------------------------------------------------------------

u32 BuildTagListOneLevel(u8 *buf, u32 size, TagList *pTagList) {

    u16 Tag;

    u16 TagSize;

    u16 lengthSize;

    u16 Length;

    u8 *CurPos, *EndPos;


    if ((buf == NULL) || (size == 0)) {

        return SDK_OK;

    }


    CurPos = buf;

    EndPos = (buf + size);


    while (CurPos < EndPos) {

        if ((*CurPos == 0x00) || (*CurPos == 0xFF)) {

            CurPos++;

            continue;

        }

        if (SDK_OK !=
            ParseTlvInfo(CurPos, (u16)(EndPos - CurPos), &Tag, &TagSize, &Length, &lengthSize)) {

            return SDK_ERR;

        }

        CurPos += TagSize + lengthSize;


        if (TagIsExisted(pTagList, Tag)) {

            pTagList->duplicateItem[pTagList->duplicateItemCount] = Tag;

            (pTagList->duplicateItemCount)++;

        }


        if (SDK_OK != AppendTagValue(Tag, CurPos, Length, pTagList)) {

            return SDK_ERR;

        }


        CurPos += Length;

    }


    if (CurPos != EndPos) {

        return SDK_ERR;

    }

    else {

        return SDK_OK;

    }

}



//------------------------------------------------------------------------------------------

void ResetDuplicateStatus(TagList *pTagList) {

    pTagList->duplicateItemCount = 0;

}



//------------------------------------------------------------------------------------------

void removeTag(TagList *pTagList, u16 tag) {

    u32 index, tagIndex;


    tagIndex = GetTagIndex(pTagList, tag);


    if (tagIndex > 0) {

        sdkFreeMem(pTagList->tagItem[tagIndex].Value);

        for (index = tagIndex; index < pTagList->ItemCount - 1; index++) {

            memcpy(&pTagList->tagItem[index], &pTagList->tagItem[index + 1], sizeof(TagItem));

        }

        (pTagList->ItemCount)--;

    }

}




