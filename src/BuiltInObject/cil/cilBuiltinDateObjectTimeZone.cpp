/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2005-2008 CRI Middleware Co,, Ltd,
 *
 *
 *
 * Library  : CRIScript
 * Module   : cilBuiltinDateObject,cpp
 * File     : Defines builtin Date object
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include <time.h>
#include "criScript.h"
#include "cilCodeGen.h"

namespace cri {

/***************************************************************************
 *      Variables
 ***************************************************************************/
	
const TIME_ZONE CDateObject::TimeZoneSettings[] = {
{ L"ACDT",	10,	30 },	// Australian Central Daylight Time
{ L"ACIT",	8,	00 },	// Ashmore and Cartier islands Time
{ L"ACST",	9,	30 },	// Australian Central Standard Time
{ L"ACT",	-5,	0 },	// Acre Time
{ L"ADT",	4,	0 },	// Arabia Daylight Time
{ L"ADT",	-3,	0 },	// Atlantic Daylight Time
{ L"AEDT",	11,	0 },	// Australian Eastern Daylight Time
{ L"AEST",	10 },	// Australian Eastern Standard Time
{ L"AFT",	4,	30 },	// Afghanistan Time
{ L"AKDT",	-8,	0 },	// Alaska Daylight Time
{ L"AKST",	-9,	0 },	// Alaska Standard Time
{ L"AMDT",	5,	0 },	// Armenia Daylight Time
{ L"AMST",	-3,	0 },	// Amazon Standard Time
{ L"AMST",	4,	0 },	// Armenia Standard Time
{ L"AMT",	-4,	0 },	// Amazon Time
{ L"ANAST",	13,	0 },	// AnadyrÅf Summer Time
{ L"ANAT",	12,	0 },	// AnadyrÅf Time
{ L"APO",	8,	15 },	// Apo island Time (an unofficial time zone ? just for fun)
{ L"ART",	-3,	0 },	// Argentina Time
{ L"ARDT",	-2,	0 },	// Argentina Daylight Time
{ L"AST",	3,	0 },	// Al Manamah Standard Time
{ L"AST",	3,	0 },	// Arabia Standard Time
{ L"AST",	3,	0 },	// Arabic Standard Time
{ L"AST",	-4,	0 },	// Atlantic Standard Time
{ L"ACWDT",	9,	45 },	// Australian Central Western Daylight Time
{ L"ACWST",	8,	45 },	// Australian Central Western Standard Time
{ L"AZODT",	0 },	// Azores Daylight Time
{ L"AZOST",	-1,	0 },	// Azores Standard Time
{ L"AZST",	5,	0 },	// Azerbaijan Summer Time
{ L"AZT",	4,	0 },	// Azerbaijan Time
{ L"BIT",	-12,	0 },	// Baker island Time
{ L"BDT",	6,	0 },	// Bangladesh Time
{ L"BDT",	8,	0 },	// Brunei Time
{ L"BIOT",	6,	0 },	// British Indian Ocean Time
{ L"BOT",	-4,	0 },	// Bolivia Time
{ L"BRST",	-2,	0 },	// Brazilia Summer Time
{ L"BRT",	-3,	0 },	// Brazilia Time
{ L"BST",	1,	0 },	// British Summer Time
{ L"BTT",	6,	0 },	// Bhutan Time
{ L"CAST",	5,	0 },	// Chinese Antarctic Standard Time
{ L"CAT",	2,	0 },	// Central Africa Time
{ L"CCT",	6,	30 },	// Cocos islands Time
{ L"CDT",	-5,	0 },	// Central Daylight Time
{ L"CEDT",	2,	0 },	// Central Europe Daylight Time
{ L"CET",	1,	0 },	// Central Europe Time
{ L"CGST",	-2,	0 },	// Central Greenland Summer Time
{ L"CGT",	-3,	0 },	// Central Greenland Time
{ L"CHADT",	13,	45 },	// Chatham island Daylight Time
{ L"CHAST",	12,	45 },	// Chatham island Standard Time
{ L"ChST",	10 },	// Chamorro Standard Time
{ L"CIST",	-8,	0 },	// Clipperton island Standard Time
{ L"CKT",	-10 },	// Cook island Time
{ L"CLDT",	-3,	0 },	// Chile Daylight Time
{ L"CLST",	-4,	0 },	// Chile Standard Time
{ L"COT",	-5,	0 },	// Colombia Time
{ L"CST",	-6,	0 },	// Central Standard Time
{ L"CST",	8,	0 },	// China Standard Time
{ L"CVT",	-1,	0 },	// Cape Verde Time
{ L"CXT",	7,	0 },	// Christmas island Time
{ L"DAVT",	7,	0 },	// Davis Time
{ L"DTAT",	10,	0 },	// District de Terre Ad?lie Time
{ L"EADT",	-5,	0 },	// Easter island Daylight Time
{ L"EAST",	-6,	0 },	// Easter island Standard Time
{ L"EAT",	3,	0 },	// East Africa Time
{ L"ECT",	-5,	0 },	// Ecuador Time
{ L"EDT",	-4,	0 },	// Eastern Daylight Time
{ L"EEST",	3,	0 },	// Eastern Europe Summer Time
{ L"EET",	2,	0 },	// Eastern Europe Time
{ L"EGT",	-1,	0 },	// Eastern Greenland Time
{ L"EGST",	0,	0 },	// Eastern Greenland Summer Time
{ L"EKST",	6,	0 },	// East Kazakhstan Standard Time
{ L"EST",	-5,	0 },	// Eastern Standard Time
{ L"FJT",	12,	0 },	// Fiji Time
{ L"FKDT",	-3,	0 },	// Falkland island Daylight Time
{ L"FKST",	-4,	0 },	// Falkland island Standard Time
{ L"FNT",	-2,	0 },	// Fernando de Noronha Time
{ L"GALT",	-6,	0 },	// Galapagos Time
{ L"GET",	4,	0 },	// Georgia Standard Time
{ L"GFT",	-3,	0 },	// French Guiana Time
{ L"GILT",	12,	0 },	// Gilbert island Time
{ L"GIT",	-9,	0 },	// Gambier island Time
{ L"GMT",	0,	0 },	// Greenwich Meantime
{ L"GST",	4,	0 },	// Gulf Standard Time
{ L"GST",	-2,	0 },	// South Georgia and the South Sandwich islands
{ L"GYT",	-4,	0 },	// Guyana Time
{ L"HADT",	-9,	0 },	// Hawaii - Aleutian Daylight Time
{ L"HAST",	-10,	0 },	// Hawaii - Aleutian Standard Time
{ L"HKST",	8,	0 },	// Hong Kong Standard Time
{ L"HMT",	5,	0 },	// Heard and McDonald islands Time
{ L"ICT",	4,	0 },	// ?les Crozet Time
{ L"ICT",	7,	0 },	// Indochina Time
{ L"IDT",	1,	0 },	// Ireland Daylight Time
{ L"IDT",	3,	0 },	// israel Daylight Time
{ L"IRKST",	9,	0 },	// Irkutsk Summer Time
{ L"IRKT",	8,	0 },	// Irkutsk Time
{ L"IRDT",	4,	30 },	// ?ran Daylight Time
{ L"IRST",	3,	30 },	// ?ran Standard Time
{ L"IST",	5,	30 },	// Indian Standard Time
{ L"IST",	0,	0 },	// Ireland Standard Time
{ L"IST",	2,	0 },	// israel Standard Time
{ L"JFDT",	-3,	0 },	// Juan Fernandez islands Daylight Time
{ L"JFST",	-4,	0 },	// Juan Fernandez islands Standard Time
{ L"JST",	9,	0 },	// Japan Standard Time
{ L"KGST",	6,	0 },	// Kyrgyzstan Summer Time
{ L"KGT",	5,	0 },	// Kyrgyzstan Time
{ L"KRAST",	8,	0 },	// Krasnoyarsk Summer Time
{ L"KRAT",	7,	0 },	// Krasnoyarsk Time
{ L"KOST",	11,	0 },	// Kosrae Standard Time
{ L"KOVT",	7,	0 },	// Khovd Time
{ L"KOVST",	8,	0 },	// Khovd Summer Time
{ L"KST",	9,	0 },	// Korea Standard Time
{ L"LHDT",	11,	0 },	// Lord Howe Daylight Time
{ L"LHST",	10,	30 },	// Lord Howe Standard Time
{ L"LINT",	14,	0 },	// Line island Time
{ L"LKT",	6,	0 },	// Sri Lanka Time
{ L"MAGST",	12,	0 },	// Magadan island Summer Time
{ L"MAGT",	11,	0 },	// Magadan island Time
{ L"MIT",	-9,	30 },	// Marquesas islands Time
{ L"MHT",	12,	0 },	// Marshall islands Time
{ L"MAWT",	6,	0 },	// Mawson Time
{ L"MMT",	6,	30 },	// Myanmar Time
{ L"MNT",	8,	0 },	// Mongolia Time
{ L"MNST",	9,	0 },	// Mongolia Summer Time
{ L"MSDT",	4,	0 },	// Moscow Daylight Time
{ L"MSST",	3,	0 },	// Moscow Standard Time
{ L"MDT",	-6,	0 },	// Mountain Daylight Time
{ L"MST",	-7,	0 },	// Mountain Standard Time
{ L"MUT",	4,	0 },	// Mauritius Time
{ L"MVT",	5,	0 },	// Maldives Time
{ L"MYT",	8,	0 },	// Malaysia Time
{ L"NCT",	11,	0 },	// New Caledonia Time
{ L"NDT",	-2,	30 },	// Newfoundland Daylight Time
{ L"NFT",	11,	30 },	// Norfolk Time
{ L"NPT",	5,	45 },	// Nepal Time
{ L"NRT",	12,	0 },	// Nauru Time
{ L"NOVST",	7,	0 },	// Novosibirsk Summer Time
{ L"NOVT",	6,	0 },	// Novosibirsk Time
{ L"NST",	-3,	30 },	// Newfoundland Standard Time
{ L"NUT",	-11,	0 },	// Niue Time
{ L"NZDT",	13,	0 },	// New Zealand Daylight Time
{ L"NZST",	12,	0 },	// New Zealand Standard Time
{ L"OMSST",	7,	0 },	// Omsk Summer Time
{ L"OMST",	6,	0 },	// Omsk Standard Time
{ L"PDT",	-7,	0 },	// Pacific Daylight Time
{ L"PETST",	13,	0 },	// Petropavlovsk Summer Time
{ L"PET",	-5,	0 },	// Peru Time
{ L"PETT",	12,	0 },	// Petropavlovsk Time
{ L"PGT",	10,	0 },	// Papua New Guinea Time
{ L"PHOT",	13,	0 },	// Phoenix island Time
{ L"PHT",	8,	0 },	// Philippines Time
{ L"PIT",	8,	0 },	// Paracel islands Time
{ L"PIT",	-6,	0 },	// Peter island Time
{ L"PKT",	5,	0 },	// Pakistan Time
{ L"PMDT",	-2,	0 },	// Pierre & Miquelon Daylight Time
{ L"PMST",	-3,	0 },	// Pierre & Miquelon Standard Time
{ L"PONT",	11,	0 },	// Pohnpei Standard Time
{ L"PST",	-8,	0 },	// Pacific Standard Time
{ L"PST",	-8,	0 },	// Pitcairn Standard Time
{ L"PWT",	9,	0 },	// Palau Time
{ L"PYST",	-3,	0 },	// Paraguay Summer Time
{ L"PYT",	-4,	0 },	// Paraguay Time
{ L"RET",	4,	0 },	// R?union Time
{ L"ROTT",	-3,	0 },	// Rothera Time
{ L"SAMST",	5,	0 },	// Samara Summer Time
{ L"SAMT",	4,	0 },	// Samara Time
{ L"SAST",	2,	0 },	// South Africa Standard Time
{ L"SBT",	11,	0 },	// Solomon island Time
{ L"SCDT",	13,	0 },	// Santa Claus Delivery Time
{ L"SCST",	12,	0 },	// Santa Claus Standard Time
{ L"SCT",	4,	0 },	// Seychelles Time
{ L"SEST",	1,	0 },	// Swedish Standard Time
{ L"SGT",	8,	0 },	// Singapore Time
{ L"SIT",	8,	0 },	// Spratly islands Time
{ L"SLT",	0,	0 },	// Sierra Leone Time
{ L"SRT",	-3,	0 },	// Suriname Time
{ L"SST",	-11,	0 },	// Samoa Standard Time
{ L"SYST",	3,	0 },	// Syrian Summer Time
{ L"SYT",	2,	0 },	// Syrian Standard Time
{ L"TAHT",	-10,	0 },	// Tahiti Time
{ L"TFT",	5,	0 },	// French Southern and Antarctic Time
{ L"TJT",	5,	0 },	// Tajikistan Time
{ L"TKT",	-10,	0 },	// Tokelau Time
{ L"TMT",	5,	0 },	// Turkmenistan Time
{ L"TOT",	13,	0 },	// Tonga Time
{ L"TPT",	9,	0 },	// East Timor Time
{ L"TRUT",	10,	0 },	// Truk Time
{ L"TVT",	12,	0 },	// Tuvalu Time
{ L"TWT",	8,	0 },	// Taiwan Time
{ L"UTC",	0,	0 },	// Universal Coordinated Time
{ L"UYT",	-3,	0 },	// Uruguay Standard Time
{ L"UYST",	-2,	0 },	// Uruguay Summer Time
{ L"UZT",	5,	0 },	// Uzbekistan Time
{ L"VLAST",	11,	0 },	// Vladivostok Summer Time
{ L"VLAT",	10,	0 },	// Vladivostok Time
{ L"VOST",	6,	0 },	// Vostok Time
{ L"VST",	-4,	30 },	// Venezuela Standard Time
{ L"VUT",	11,	0 },	// Vanuatu Time
{ L"WAST",	2,	0 },	// Western Africa Summer Time
{ L"WAT",	1,	0 },	// Western Africa Time
{ L"WEST",	1,	0 },	// Western Europe Summer Time
{ L"WET",	0,	0 },	// Western Europe Time
{ L"WFT",	12,	0 },	// Wallis and Futuna Time
{ L"WKST",	5,	0 },	// West Kazakhstan Standard Time
{ L"WDT",	9,	0 },	// Western Australia Daylight Time
{ L"WST",	8,	0 },	// Western Australia Standard Time
{ L"WIB",	7,	0 },	// Waktu Indonesia Bagian Barat
{ L"WITA",	8,	0 },	// Waktu Indonesia Bagian Tengah
{ L"WIT",	9,	0 },	// Waktu Indonesia Bagian Timur
{ L"YAKST",	10,	0 },	// Yakutsk Summer Time
{ L"YAKT",	9,	0 },	// Yakutsk Time
{ L"YAPT",	10,	0 },	// Yap Time
{ L"YEKST",	6,	0 },	// Yekaterinburg Summer Time
{ L"YEKT",	5,	0 },	// Yekaterinburg Time
};

void CDateObject::setTimeZone( wchar_t* pstrTimeZone )
{
	int32_t iIndex;

	int32_t iLow = 0;
	int32_t iHigh = ARRAYSIZE( TimeZoneSettings );
	while( iLow < iHigh) {
		iIndex = (iLow + iHigh)/2;
		int32_t iResult = wcscmp( TimeZoneSettings[ iIndex ].strTimeZone, pstrTimeZone );
		if( iResult == 0 )
		{
			m_currentTimeZoneIndex = iIndex;
			m_LocalTZA = ( TimeZoneSettings[ iIndex ].cHour * MinutesPerHour
				+ TimeZoneSettings[ iIndex ].cMinute
					* (TimeZoneSettings[ iIndex ].cHour < 0 ? -1 : 1 ) ) 
					* msPerMinute;

			return;
		}

		if( iResult < 0 )
		   iLow = iIndex + 1; 
		else
			iHigh = iIndex; 
	}

	return;	// not found  
}

} //namespace CRI