/*
 * aSub routine backing $(PLCNAME):MODEL. Looks up the CIP Product Code read
 * from the PLC's identity object (via $(PLCNAME):MODELRAW) and resolves it
 * to the system_tag string operators see.
 *
 * Based on Rockwell's own FactoryTalk Logix Echo firmware catalog
 * CIP identity table.
 */
#include <cstring>
#include <map>
#include <string>

#include <aSubRecord.h>
#include <registryFunction.h>
#include <epicsExport.h>

namespace {

struct RawEntry {
    long code;
    const char *tag;
};

const RawEntry rawModelTable[] = {
    { 3, "CTL_LTX_L5550" },  /* tested, code not independently confirmed */
    { 51, "CTL_LGX_5555" },  /* tested; 1756-L55 */
    { 54, "CTL_LGX_L61" },   /* tested; 1756-L61 */
    { 77, "CMP_LGX_L32" },   /* tested; 1769-L32E */
    { 15, "SFT_LGX_5800" },  /* SoftLogix5800 */
    { 40, "CTL_LGX_L1" },    /* 1756-L1 */
    { 43, "CMP_LGX_L20" },   /* 1769-L20 */
    { 44, "CMP_LGX_L30" },   /* 1769-L30 */
    { 49, "PF_DLX_5725" },   /* PowerFlex-DriveLogix5725 */
    { 50, "CTL_LGX_L53" },   /* 1756-L53 */
    { 52, "PF_DLX_5730" },   /* PowerFlex-DriveLogix5730 */
    { 53, "EMU_LGX_STUDIO5000" },  /* Studio5000-LogixEmulate */
    { 55, "CTL_LGX_L62" },   /* 1756-L62 */
    { 56, "CTL_LGX_L63" },   /* 1756-L63 */
    { 57, "CTL_LGX_L64" },   /* 1756-L64 */
    { 64, "CMP_LGX_L31" },   /* 1769-L31 */
    { 65, "CMP_LGX_L35E" },  /* 1769-L35E */
    { 67, "CTL_LGX_L61S" },  /* 1756-L61S */
    { 68, "CTL_LGX_L62S" },  /* 1756-L62S */
    { 69, "CTL_LGX_LSP" },   /* 1756-LSP */
    { 72, "CMP_LGX_L43" },   /* 1768-L43 */
    { 74, "CMP_LGX_L45" },   /* 1768-L45 */
    { 76, "CMP_LGX_L32C" },  /* 1769-L32C */
    { 78, "CMP_LGX_L35C" },  /* 1769-L35C */
    { 79, "CTL_LGX_L60M03SE" },  /* 1756-L60M03SE */
    { 80, "CMP_LGX_L35CR" },  /* 1769-L35CR */
    { 85, "CTL_LGX_L65" },   /* 1756-L65 */
    { 86, "CTL_LGX_L63S" },  /* 1756-L63S */
    { 87, "CMP_LGX_L23E_QB1" },  /* 1769-L23E-QB1 */
    { 88, "CMP_LGX_L23_QBFC1" },  /* 1769-L23-QBFC1 */
    { 89, "CMP_LGX_L23E_QBFC1" },  /* 1769-L23E-QBFC1 */
    { 92, "CTL_LGX_L71" },   /* 1756-L71 */
    { 93, "CTL_LGX_L72" },   /* 1756-L72 */
    { 94, "CTL_LGX_L73" },   /* 1756-L73 */
    { 95, "CTL_LGX_L74" },   /* 1756-L74 */
    { 96, "CTL_LGX_L75" },   /* 1756-L75 */
    { 101, "CMP_LGX_L43S" },  /* 1768-L43S */
    { 102, "CMP_LGX_L45S" },  /* 1768-L45S */
    { 106, "CMP_LGX_L30ER" },  /* 1769-L30ER */
    { 107, "CMP_LGX_L33ER" },  /* 1769-L33ER */
    { 108, "CMP_LGX_L36ERM" },  /* 1769-L36ERM */
    { 109, "CMP_LGX_L30ER_NSE" },  /* 1769-L30ER-NSE */
    { 110, "CMP_LGX_L33ERM" },  /* 1769-L33ERM */
    { 146, "CTL_LGX_L7SP" },  /* 1756-L7SP */
    { 147, "CTL_LGX_L72S" },  /* 1756-L72S */
    { 148, "CTL_LGX_L73S" },  /* 1756-L73S */
    { 149, "CMP_LGX_L24ER_QB1B" },  /* 1769-L24ER-QB1B */
    { 150, "CMP_LGX_L24ER_QBFC1B" },  /* 1769-L24ER-QBFC1B */
    { 151, "CMP_LGX_L27ERM_QBFC1B" },  /* 1769-L27ERM-QBFC1B */
    { 152, "CMP_LGX_L19ER_BB1B" },  /* 1769-L19ER-BB1B */
    { 153, "CMP_LGX_L16ER_BB1B" },  /* 1769-L16ER-BB1B */
    { 154, "CMP_LGX_L18ER_BB1B" },  /* 1769-L18ER-BB1B */
    { 155, "CMP_LGX_L18ERM_BB1B" },  /* 1769-L18ERM-BB1B */
    { 156, "CMP_LGX_L30ERM" },  /* 1769-L30ERM */
    { 158, "CTL_LGX_L71S" },  /* 1756-L71S */
    { 164, "CTL_LGX_L81E" },  /* 1756-L81E */
    { 165, "CTL_LGX_L82E" },  /* 1756-L82E */
    { 166, "CTL_LGX_L83E" },  /* 1756-L83E */
    { 167, "CTL_LGX_L84E" },  /* 1756-L84E */
    { 168, "CTL_LGX_L85E" },  /* 1756-L85E */
    { 171, "CTL_LGX_L8SP" },  /* 1756-L8SP */
    { 176, "CMP_LGX_L30ERMS" },  /* 1769-L30ERMS */
    { 177, "CMP_LGX_L33ERMS" },  /* 1769-L33ERMS */
    { 178, "CMP_LGX_L36ERMS" },  /* 1769-L36ERMS */
    { 186, "CMP_LGX_L46ERMW" },  /* 5069-L46ERMW */
    { 188, "CMP_LGX_L310ER_NSE" },  /* 5069-L310ER-NSE */
    { 189, "CMP_LGX_L306ERM" },  /* 5069-L306ERM */
    { 190, "CMP_LGX_L310ERM" },  /* 5069-L310ERM */
    { 192, "CMP_LGX_L320ERM" },  /* 5069-L320ERM */
    { 193, "CMP_LGX_L330ERM" },  /* 5069-L330ERM */
    { 194, "CMP_LGX_L340ERM" },  /* 5069-L340ERM */
    { 195, "CMP_LGX_L350ERM" },  /* 5069-L350ERM */
    { 196, "CMP_LGX_L306ER" },  /* 5069-L306ER */
    { 201, "CTL_LGX_L81E_NSE" },  /* 1756-L81E-NSE */
    { 202, "CTL_LGX_L82E_NSE" },  /* 1756-L82E-NSE */
    { 203, "CTL_LGX_L83E_NSE" },  /* 1756-L83E-NSE */
    { 204, "CTL_LGX_L84E_NSE" },  /* 1756-L84E-NSE */
    { 205, "CTL_LGX_L85E_NSE" },  /* 1756-L85E-NSE */
    { 211, "CTL_LGX_L81ES" },  /* 1756-L81ES */
    { 212, "CTL_LGX_L82ES" },  /* 1756-L82ES */
    { 213, "CTL_LGX_L83ES" },  /* 1756-L83ES */
    { 214, "CTL_LGX_L84ES" },  /* 1756-L84ES */
    { 216, "CMP_LGX_L310ER" },  /* 5069-L310ER */
    { 217, "CMP_LGX_L320ER" },  /* 5069-L320ER */
    { 218, "CMP_LGX_L330ER" },  /* 5069-L330ER */
    { 219, "CMP_LGX_L340ER" },  /* 5069-L340ER */
    { 220, "CMP_LGX_L350ER" },  /* 5069-L350ER */
    { 221, "CMP_LGX_L310ERMS2" },  /* 5069-L310ERMS2 */
    { 222, "CMP_LGX_L320ERMS2" },  /* 5069-L320ERMS2 */
    { 223, "CMP_LGX_L330ERMS2" },  /* 5069-L330ERMS2 */
    { 224, "CMP_LGX_L340ERMS2" },  /* 5069-L340ERMS2 */
    { 225, "CMP_LGX_L350ERMS2" },  /* 5069-L350ERMS2 */
    { 226, "CMP_LGX_L306ERMS2" },  /* 5069-L306ERMS2 */
    { 228, "CMP_LGX_L380ERM" },  /* 5069-L380ERM */
    { 229, "CMP_LGX_L380ERMS2" },  /* 5069-L380ERMS2 */
    { 230, "CMP_LGX_L3100ERM" },  /* 5069-L3100ERM */
    { 231, "CMP_LGX_L3100ERMS2" },  /* 5069-L3100ERMS2 */
    { 233, "CMP_LGX_L37ERMO" },  /* 1769-L37ERMO */
    { 234, "CMP_LGX_L37ERMOS" },  /* 1769-L37ERMOS */
    { 235, "CMP_LGX_L306ERS2" },  /* 5069-L306ERS2 */
    { 236, "CMP_LGX_L310ERS2" },  /* 5069-L310ERS2 */
    { 237, "CMP_LGX_L320ERS2" },  /* 5069-L320ERS2 */
    { 238, "CMP_LGX_L330ERS2" },  /* 5069-L330ERS2 */
    { 239, "CMP_LGX_L340ERS2" },  /* 5069-L340ERS2 */
    { 240, "CMP_LGX_L350ERS2" },  /* 5069-L350ERS2 */
    { 241, "CMP_LGX_L380ERS2" },  /* 5069-L380ERS2 */
    { 242, "CMP_LGX_L3100ERS2" },  /* 5069-L3100ERS2 */
    { 243, "CMP_LGX_L306ERMS3" },  /* 5069-L306ERMS3 */
    { 244, "CMP_LGX_L310ERMS3" },  /* 5069-L310ERMS3 */
    { 245, "CMP_LGX_L320ERMS3" },  /* 5069-L320ERMS3 */
    { 246, "CMP_LGX_L330ERMS3" },  /* 5069-L330ERMS3 */
    { 247, "CMP_LGX_L340ERMS3" },  /* 5069-L340ERMS3 */
    { 248, "CMP_LGX_L350ERMS3" },  /* 5069-L350ERMS3 */
    { 249, "CMP_LGX_L380ERMS3" },  /* 5069-L380ERMS3 */
    { 250, "CMP_LGX_L3100ERMS3" },  /* 5069-L3100ERMS3 */
    { 255, "CMP_LGX_L38ERM" },  /* 1769-L38ERM */
    { 256, "CMP_LGX_L38ERMS" },  /* 1769-L38ERMS */
    { 257, "CMP_LGX_L37ERM" },  /* 1769-L37ERM */
    { 258, "CMP_LGX_L37ERMS" },  /* 1769-L37ERMS */
    { 282, "CTL_LGX_L81EP" },  /* 1756-L81EP */
    { 283, "CTL_LGX_L83EP" },  /* 1756-L83EP */
    { 284, "CTL_LGX_L85EP" },  /* 1756-L85EP */
    { 285, "CMP_LGX_L320ERP" },  /* 5069-L320ERP */
    { 286, "CMP_LGX_L340ERP" },  /* 5069-L340ERP */
    { 290, "CMP_LGX_L4100ERMW" },  /* 5069-L4100ERMW */
    { 291, "CMP_LGX_L450ERMW" },  /* 5069-L450ERMW */
    { 292, "CMP_LGX_L4200ERMW" },  /* 5069-L4200ERMW */
    { 293, "CMP_LGX_L430ERMW" },  /* 5069-L430ERMW */
};

std::map<long, std::string> buildModelTable()
{
    std::map<long, std::string> table;
    const size_t n = sizeof(rawModelTable) / sizeof(rawModelTable[0]);
    for (size_t i = 0; i < n; i++) {
        table.insert(std::make_pair(rawModelTable[i].code, std::string(rawModelTable[i].tag)));
    }
    return table;
}

const std::map<long, std::string> modelTable = buildModelTable();

} // namespace

extern "C" {

static long plcAdminModelLookupInit(aSubRecord *prec)
{
    return 0;
}

static long plcAdminModelLookup(aSubRecord *prec)
{
    const long code = static_cast<long>(*static_cast<const double *>(prec->a));

    std::string tag = "UNKNOWN";
    const std::map<long, std::string>::const_iterator it = modelTable.find(code);
    if (it != modelTable.end()) {
        tag = it->second;
    }

    char *out = static_cast<char *>(prec->vala);
    std::strncpy(out, tag.c_str(), MAX_STRING_SIZE - 1);
    out[MAX_STRING_SIZE - 1] = '\0';

    return 0;
}

epicsRegisterFunction(plcAdminModelLookupInit);
epicsRegisterFunction(plcAdminModelLookup);

} // extern "C"
