#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerStatusPunchWazaPowerBase(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {

    if (Common::GetEventVar(args, EventVar::Label::POKEID_ATK) != pokeID)
        return;

    BTL_POKEPARAM::Object* bpp = Common::GetPokeParam(args, Common::GetEventVar(args, EventVar::Label::POKEID_DEF));
    Pml::WazaData::WazaSick sick = GetFirstMatchingSick(bpp);

    if (sick == Pml::WazaData::WazaSick::WAZASICK_NONE)
        return;

    Common::MulEventVar(args, EventVar::Label::WAZA_POWER_RATIO, FX32::CONST_2_0);
}

void HandlerStatusPunchWazaParam(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
    // Type logic
}
constexpr Pml::WazaData::WazaSick STATUS_PUNCH_PRIORITY[] =
{
    Pml::WazaData::WazaSick::WAZASICK_MAHI,
    Pml::WazaData::WazaSick::WAZASICK_NEMURI,
    Pml::WazaData::WazaSick::WAZASICK_KOORI,
    Pml::WazaData::WazaSick::WAZASICK_YAKEDO,
    Pml::WazaData::WazaSick::WAZASICK_DOKU,
    Pml::WazaData::WazaSick::WAZASICK_KONRAN,
    Pml::WazaData::WazaSick::WAZASICK_NOROI,
    Pml::WazaData::WazaSick::WAZASICK_MEROMERO,
    Pml::WazaData::WazaSick::WAZASICK_BIND,
    Pml::WazaData::WazaSick::WAZASICK_YADORIGI,
};

Pml::WazaData::WazaSick GetFirstMatchingSick(BTL_POKEPARAM::Object* bpp)
{
    for (auto sick : STATUS_PUNCH_PRIORITY)
    {
        if (bpp->CheckSick(sick))
            return sick;
    }

    return Pml::WazaData::WazaSick::WAZASICK_NONE;
}

EventFactor::EventHandlerTable::Array* ADD_StatusPunch() {
    EventFactor::EventHandlerTable::Array* table = getExtraMoveHandlers()->HandlerTable_StatusPunch;
    if (table == nullptr) {
        table = CreateMoveEventHandlerTable(2);
        table->m_Items[0] = CreateMoveEventHandler(EventID::WAZA_POWER, (Il2CppMethodPointer)&HandlerStatusPunchWazaPowerBase());
        table->m_Items[1] = CreateMoveEventHandler(EventID::WAZA_PARAM, (Il2CppMethodPointer)&HandlerStatusPunchWazaParam());
        getExtraMoveHandlers()->HandlerTable_StatusPunch = table;
    }
    return table;
}

void Handlers_Move_StatusPunch(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "StatusPunch"), (Il2CppMethodPointer)&ADD_StatusPunch);
    getExtraMoveHandlers()->currentIndex++;
}