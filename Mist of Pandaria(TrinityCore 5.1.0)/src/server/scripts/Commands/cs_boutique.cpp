#include "ScriptMgr.h"
#include "Chat.h"
#include "Channel.h"
#include "ObjectMgr.h"
#include "Config.h"
#include "Player.h"
#include "ReputationMgr.h"
#include <cctype>


class boutique_cs : public CommandScript
{
public:
    boutique_cs() : CommandScript("cs_boutique") {}

    ChatCommand *GetCommands() const
    {

        static ChatCommand boutiqueCommandTable [] =
        {
            {"rename", SEC_PLAYER, false, &HandleBoutiqueRenameCommand, "Utiliser le service de renommage de personnage", NULL},
            {"po", SEC_PLAYER, false, HandleBoutiquePOCommand, "Utiliser le service d'achat de po", NULL},
            {"level", SEC_PLAYER, false, HandleBoutiqueLevelCommand, "Utiliser le service d'ajout de niveaux", NULL},
            {"item", SEC_PLAYER, false, HandleBoutiqueItemCommand, "Utiliser le service d'achat d'objets", NULL},
            {"recup", SEC_PLAYER, false, HandleBoutiqueRecupCommand, "Valider une recuperation", NULL},
            {"race", SEC_PLAYER, false, HandleBoutiqueRaceCommand, "Utiliser le service de changement de race", NULL},
            {"faction", SEC_PLAYER, false, HandleBoutiqueFactionCommand, "Utiliser le service de changement de faction", NULL},
            {NULL, 0, false, NULL, "", NULL}
        };

        static ChatCommand cmd [] =
        {
            {"boutique", SEC_PLAYER, false, NULL, "", boutiqueCommandTable},
            {NULL, 0, false, NULL, "", NULL}
        };

        return cmd;
    }

    static bool HandleBoutiqueRenameCommand(ChatHandler *handler, const char *args)
    {
        const char* reqcount = "SELECT id FROM boutique_service_achat WHERE accountId='%u' AND type=0 AND recup=0 LIMIT 1";

        sLog->outDebug(LOG_FILTER_NETWORKIO, reqcount, handler->GetSession()->GetAccountId());
        QueryResult resultcount = LoginDatabase.PQuery(reqcount, handler->GetSession()->GetAccountId());
        if (!resultcount) {
            //Vous ne disposez actuellement d'aucun service de ce type. Vous pouvez acheter ce service sur la boutique
            handler->PSendSysMessage(11016);
            return true;
        }
		
		
        const char* qmask = "SELECT 1 FROM boutique_service WHERE realmMask & '%u' != 0 and type = 0";
        QueryResult reqmask = LoginDatabase.PQuery(qmask, (1<<(realmID-1)));
        if (!reqmask) {
            //Ce produit ou service n'est pas disponible pour ce royaume.
            handler->PSendSysMessage(11018);
            return true;
        }
		
        Field *fieldscount = resultcount->Fetch();
        int id = fieldscount[0].GetInt32();

        Player* target = handler->GetSession()->GetPlayer();
        if (target) {
            //Le changement de nom a Ã©tÃ© demandÃ© pour le personnage %s.
            //Son nouveau nom vous sera demandÃ© lors de la prochaine tentative de connexion
            //Bonne continuation sur Aurore
            handler->PSendSysMessage(11017);
            target->SetAtLoginFlag(AT_LOGIN_RENAME);

            const char* requpdate = "UPDATE boutique_service_achat SET realmID = '%u', recup='%u' WHERE id='%u'";
            LoginDatabase.PExecute(requpdate, realmID, (uint32)handler->GetSession()->GetPlayer()->GetGUID(), id);
        }

        return true;
    }

    static bool HandleBoutiqueLevelCommand(ChatHandler *handler, const char *args) {
        const char* reqcount = "SELECT id FROM boutique_service_achat WHERE accountId='%u' AND type=1 AND recup=0 LIMIT 1";

        sLog->outDebug(LOG_FILTER_NETWORKIO, reqcount, handler->GetSession()->GetAccountId());
        QueryResult resultcount = LoginDatabase.PQuery(reqcount, handler->GetSession()->GetAccountId());

        if (!resultcount) {
            //Vous ne disposez actuellement d'aucun service de ce type. Vous pouvez acheter ce service sur la boutique
            handler->PSendSysMessage(11016);
            return true;
        }
		
        Field* fieldscount = resultcount->Fetch();
		
        const char* qmask = "SELECT 1 FROM boutique_service WHERE realmMask & '%u' != 0 and type = 1";
        QueryResult reqmask = LoginDatabase.PQuery(qmask, (1<<(realmID-1)));
        if (!reqmask) {
            //Ce produit ou service n'est pas disponible pour ce royaume.
            handler->PSendSysMessage(11018);
            return true;
        }

        int id = fieldscount[0].GetInt32();

        Player* target = handler->GetSession()->GetPlayer();
        if (target) {
            int levelToAdd = 0;
            int level = target->getLevel();

            if (level<40) levelToAdd = 8;
            else if (level<50) levelToAdd = 8;
            else if (level<60) levelToAdd = 7;
            else if (level<70) levelToAdd = 6;
            else if (level<75) levelToAdd = 5;
            else if (level<80) levelToAdd = 4;
            else if (level<83) levelToAdd = 3;
            else if (level<86) levelToAdd = 2;
            else if (level<90) levelToAdd = 1;
            else {
                return false;
            }

            uint32 oldlevel = target->getLevel();
            uint32 newlevel = target->getLevel()+levelToAdd;

            if (target)
            {
                target->GiveLevel(newlevel);
                target->InitTalentForLevel();
                target->SetUInt32Value(PLAYER_XP,0);

                if (handler->needReportToTarget(target))
                {
                    if (oldlevel == newlevel)
                        handler->PSendSysMessage(559,handler->GetNameLink().c_str());
                    else if (oldlevel < newlevel)
                        handler->PSendSysMessage(557,handler->GetNameLink().c_str(),newlevel);
                    else // if (oldlevel > newlevel)
                        handler->PSendSysMessage(558,handler->GetNameLink().c_str(),newlevel);
                }
            }
            else
            {
                // update level and XP at level, all other will be updated at loading
                CharacterDatabase.PExecute("UPDATE characters SET level = '%u', xp = 0 WHERE guid = '%u'", newlevel, GUID_LOPART(target->GetGUIDLow()));
            }

            target->SaveToDB();

            const char* requpdate = "UPDATE boutique_service_achat SET realmID = '%u', recup='%u' WHERE id='%u'";
            LoginDatabase.PExecute(requpdate, realmID, (uint32)handler->GetSession()->GetPlayer()->GetGUID(), id);
        }

        return true;
    }

    static bool HandleBoutiqueItemCommand(ChatHandler *handler, const char *args) {
        uint32 achatId = (uint32)atol(args);
        if (!achatId) {
            handler->PSendSysMessage(11007, handler->GetSession()->GetPlayerName().c_str());
            return false;
        }

        const char* reqcount = "SELECT nom, itemId, recup, quantite FROM boutique_achat WHERE accountId = '%u' AND id='%u'";

        QueryResult resultcount = LoginDatabase.PQuery(reqcount, handler->GetSession()->GetAccountId(), achatId);

        if (!resultcount) {
            //Cet achat n'existe pas, ne vous est pas attribue ou n'est pas disponible sur ce serveur.
            handler->PSendSysMessage(11008);
            return true;
        }

        Field* fieldscount = resultcount->Fetch();
		

        //Vous avez dÃ©jÃ  attribuÃ© cet achat Ã  un de vos personnages
        if (fieldscount[2].GetInt32()!=0) {
            const char* reqperso = "SELECT count(*), name FROM characters WHERE guid='%u'";
            QueryResult resultperso = CharacterDatabase.PQuery(reqperso, fieldscount[2].GetInt32());
            Field* fieldperso = resultperso->Fetch();
            handler->PSendSysMessage(11014, fieldperso[1].GetCString());
        }

        uint32 itemId = fieldscount[1].GetInt32();
		
        const char* qmask = "SELECT 1 FROM boutique_produit WHERE realmMask & '%u' != 0 and itemId = '%u'";
        QueryResult reqmask = LoginDatabase.PQuery(qmask, (1<<(realmID-1)), itemId);
        if (!reqmask) {
            //Ce produit ou service n'est pas disponible pour ce royaume.
            handler->PSendSysMessage(11018);
            return true;
        }

        ItemTemplate const *tmp = sObjectMgr->GetItemTemplate(itemId);

        if (!tmp) {
            handler->PSendSysMessage(11011); //Cet objet n'existe pas. Veuillez contacter un MJ et lui dÃ?Â©crire le problÃ?Â¨me
            return false;
        }

        //Adding items
        uint32 noSpaceForCount = 0;

        Player* plTarget = handler->GetSession()->GetPlayer();
        Player* pl = plTarget;
        uint32 count = fieldscount[3].GetInt32();

        // check space and find places
        ItemPosCountVec dest;
        uint8 msg = plTarget->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, count, &noSpaceForCount);
        if (msg != EQUIP_ERR_OK)                               // convert to possible store amount
            count -= noSpaceForCount;

        if (count == 0 || dest.empty())                         // can't add any
        {
            handler->PSendSysMessage(11015);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Item* item = plTarget->StoreNewItem(dest, itemId, true, Item::GenerateItemRandomPropertyId(itemId));
        for (ItemPosCountVec::const_iterator itr = dest.begin(); itr != dest.end(); ++itr) {
            if (Item* item1 = pl->GetItemByPos(itr->pos)) {
                item1->SetBinding(true);
            }
        }

        if (count > 0 && item)
        {
            plTarget->SendNewItem(item,count,true,false);
            const char* requpdate = "UPDATE boutique_achat SET realmID = '%u', recup='%u' WHERE id='%u'";
            LoginDatabase.PExecute(requpdate, realmID, (uint32)handler->GetSession()->GetPlayer()->GetGUIDLow(), achatId);

            handler->PSendSysMessage(11013, fieldscount[0].GetCString());
        }

        if (noSpaceForCount > 0)
            handler->PSendSysMessage(11015);

        plTarget->SaveToDB();
        return true;
    }


    static bool HandleBoutiquePOCommand(ChatHandler *handler, const char *args) {
        const char* reqcount = "SELECT id FROM boutique_service_achat WHERE accountId='%u' AND type=2 AND recup=0 LIMIT 1";

        sLog->outDebug(LOG_FILTER_NETWORKIO, reqcount, handler->GetSession()->GetAccountId());
        QueryResult resultcount = LoginDatabase.PQuery(reqcount, handler->GetSession()->GetAccountId());

        if (!resultcount) {
            //Vous ne disposez actuellement d'aucun service de ce type. Vous pouvez acheter ce service sur la boutique
            handler->PSendSysMessage(11016);
            return true;
        }        

		
        const char* qmask = "SELECT 1 FROM boutique_service WHERE realmMask & '%u' != 0 and type = 2";
        QueryResult reqmask = LoginDatabase.PQuery(qmask, (1<<(realmID-1)));
        if (!reqmask) {
            //Ce produit ou service n'est pas disponible pour ce royaume.
            handler->PSendSysMessage(11018);
            return true;
        }
		
        Field *fieldscount = resultcount->Fetch();
        int id = fieldscount[0].GetInt32();

        sLog->outDebug(LOG_FILTER_NETWORKIO, "NOBODIE boutique po %u", fieldscount[0].GetInt32());

        Player* target = handler->GetSession()->GetPlayer();
        if (target) {
            target->SetMoney(target->GetMoney()+(2000*100*100));

            target->SaveToDB();
            const char* requpdate = "UPDATE boutique_service_achat SET realmID = '%u', recup='%u' WHERE id='%u'";
            LoginDatabase.PExecute(requpdate, realmID, (uint32)handler->GetSession()->GetPlayer()->GetGUIDLow(), id);
        }

        return true;
    }

    static bool HandleBoutiqueRecupCommand(ChatHandler* handler, const char* args) {
        /*const uint64 playerGuid = handler->GetSession()->GetPlayer()->GetGUID();

        const char* reqcount = "SELECT count(*) FROM site_recuperations_cata WHERE idperso='%u'";
        QueryResult resultcount = LoginDatabase.PQuery(reqcount, playerGuid);
        Field* fieldscount = resultcount->Fetch();
        if (fieldscount[0].GetInt32()==0) {
            //Aucune rÃ?Â©cupÃ?Â©ration n'est disponible pour ce personnage
            handler->PSendSysMessage(11106, handler->GetSession()->GetPlayerName());
            return true;
        }

        //                        0   1           2        3               4       5      6      7            8                  9                         10           11                 12                        13          14                15                       16          17                18                       19          20                21                       22                  23                         24                    25                           26             27                    28              29                     30                    31                           32                   33                     34    35    36    37    38
        const char* req = "SELECT id, validation, idperso, niveauattribue, pvppve, stuff, monte, metierprin1, niveaumetierprin1, niveaumetierprin1palier, metierprin2, niveaumetierprin2, niveaumetierprin2palier, metiersec1, niveaumetiersec1, niveaumetiersec1palier, metiersec2, niveaumetiersec2, niveaumetiersec2palier, metiersec3, niveaumetiersec3, niveaumetiersec3palier, reputgardienshyjal, reputgardienshyjalpallier, reputcercleterrestre, reputcercleterrestrepallier, reputramkahen, reputramkahenpallier, reputtherazane, reputtherazanepallier, reputgardiensbaradin, reputgardiensbaradinpallier, reputpoinghurlenfer, reputpoinghurlenferpallier, sac1, sac2, sac3, sac4, monnaie FROM site_recuperations_cata WHERE idperso='%u' ORDER BY valid_date DESC LIMIT 1;";

        QueryResult result = SiteDatabase.PQuery(req, playerGuid);
        Field* fields = result->Fetch();
        uint32 recupId = fields[0].GetUInt32();
        uint32 validation = fields[1].GetUInt32();

        switch (validation) {
        case 0:
        case 1:
            //La rÃ?Â©cupÃ?Â©ration du personnage %s est en cours de validation par un MaÃ?Â®tre du Jeu. Reessayez plus tard
            handler->PSendSysMessage(11101, handler->GetSession()->GetPlayerName());
            return true;
            break;
        case 2:
            //La rÃ?Â©cupÃ?Â©ration du personnage %s a Ã?Â©tÃ?Â© invalidÃ?Â©e par un MaÃ?Â®tre du Jeu. Consultez votre profil sur le site pour plus d'information.
            handler->PSendSysMessage(11102, handler->GetSession()->GetPlayerName());
            return true;
            break;
        case 3:
            //La rÃ?Â©cupÃ?Â©ration du personnage %s a Ã?Â©tÃ?Â© validÃ?Â©e partiellement par un MaÃ?Â®tre du Jeu. Consultez votre profil sur le site pour plus d'information.
            handler->PSendSysMessage(11103, handler->GetSession()->GetPlayerName());
            break;
        case 4:
            //La rÃ?Â©cupÃ?Â©ration du personnage %s a Ã?Â©tÃ?Â© validÃ?Â©e par un MaÃ?Â®tre du Jeu. Vous allez a prÃ?Â©sent recevoir votre Ã?Â©quipement ainsi que les compÃ?Â©tences demandÃ?Â©es.
            handler->PSendSysMessage(11104, handler->GetSession()->GetPlayerName());
            break;
        case 5:
            //La rÃ?Â©cupÃ?Â©ration du personnage %s a dÃ?Â©jÃ?  Ã?Â©tÃ?Â© effectuÃ?Â©e.
            handler->PSendSysMessage(11105, handler->GetSession()->GetPlayerName());
            return true;
            break;
        default:
            //Aucune rÃ?Â©cupÃ?Â©ration n'est disponible pour ce personnage
            handler->PSendSysMessage(11106, handler->GetSession()->GetPlayerName());
            return true;
            break;
        }


        if (validation==3 || validation==4) {
            //Update de la base pour terminer la rÃ?Â©cupÃ?Â©ration (validation = 5)
            const char* requpdate = "UPDATE site_recuperations_cata SET validation='5' WHERE id='%u'";
            SiteDatabase.PExecute(requpdate, recupId);

            //recupÃ?Â©ration du joueur
            Player* player = handler->GetSession()->GetPlayer();

            //niveau
            uint32 niveauattrib = fields[3].GetInt32();
            int32 oldlevel = player->getLevel();
            if(niveauattrib>oldlevel) {
                if (niveauattrib<2) niveauattrib=2;
                if (niveauattrib>STRONG_MAX_LEVEL) niveauattrib = STRONG_MAX_LEVEL;
                player->GiveLevel(niveauattrib);
                player->InitTalentForLevel();
                player->SetUInt32Value(PLAYER_XP, 0);
            }

            //competence de monte
            uint32 monte = fields[6].GetInt32();
            if (monte>=75) { //Apprentice
                uint32 competence = 33388;
                if (sSpellStore.LookupEntry(competence) && !player->HasSpell(competence)) {
                    SpellEntry const* monteCompetenceInfo= sSpellStore.LookupEntry(competence);
                    if (monteCompetenceInfo && SpellMgr::IsSpellValid(monteCompetenceInfo, player)){
                        player->learnSpell(competence, false);
                    }
                }
            }
            if (monte>=150) { //Journeyman
                uint32 competence = 33391;
                if (sSpellStore.LookupEntry(competence) && !player->HasSpell(competence)) {
                    SpellEntry const* monteCompetenceInfo= sSpellStore.LookupEntry(competence);
                    if (monteCompetenceInfo && SpellMgr::IsSpellValid(monteCompetenceInfo, player)){
                        player->learnSpell(competence, false);
                    }
                }
            }
            if (monte>=225) { //Expert
                uint32 competence = 34090;
                if (sSpellStore.LookupEntry(competence) && !player->HasSpell(competence)) {
                    SpellEntry const* monteCompetenceInfo= sSpellStore.LookupEntry(competence);
                    if (monteCompetenceInfo && SpellMgr::IsSpellValid(monteCompetenceInfo, player)){
                        player->learnSpell(competence, false);
                    }
                }
            }
            if (monte>=300) { //Artisan
                uint32 competence = 34091;
                if (sSpellStore.LookupEntry(competence) && !player->HasSpell(competence)) {
                    SpellEntry const* monteCompetenceInfo= sSpellStore.LookupEntry(competence);
                    if (monteCompetenceInfo && SpellMgr::IsSpellValid(monteCompetenceInfo, player)){
                        player->learnSpell(competence, false);
                    }
                }
            }
            if (monte>=375) { //Master
                uint32 competence = 90265;
                if (sSpellStore.LookupEntry(competence) && !player->HasSpell(competence)) {
                    SpellEntry const* monteCompetenceInfo= sSpellStore.LookupEntry(competence);
                    if (monteCompetenceInfo && SpellMgr::IsSpellValid(monteCompetenceInfo, player)){
                        player->learnSpell(competence, false);
                    }
                }
            }


            //metiers
            for (int i=0 ; i<5; i++) {
                uint32 skill = fields[7+3*i].GetInt32();
                uint32 niveau = fields[7+3*i+1].GetInt32();
                uint32 palier = fields[7+3*i+2].GetInt32();

                if (niveau>palier) niveau = palier;
                if (niveau<1) niveau = 1;

                const char* req = "SELECT count(*), competence FROM site_recuperations_cata_metiers WHERE id = '%u' AND niveau = '%u';";

                QueryResult metierres = SiteDatabase.PQuery(req, skill, palier);

                Field* metierfields = metierres->Fetch();
                if (metierfields[0].GetInt32() != 0) {
                    uint32 competence =  metierfields[1].GetInt32();

                    // .learn competence
                    if (sSpellStore.LookupEntry(competence) && !player->HasSpell(competence)) {
                        SpellEntry const* metierCompetenceInfo= sSpellStore.LookupEntry(competence);

                        if (metierCompetenceInfo && SpellMgr::IsSpellValid(metierCompetenceInfo, player)){
                            player->learnSpell(competence, false);
                        }
                    }

                    // .setskill command
                    if(SkillLineEntry const* sl = sSkillLineStore.LookupEntry(skill)) {
                        if (player->GetSkillValue(skill)) {
                            player->SetSkill(uint16(skill), player->GetSkillStep(uint16(skill)), uint16(niveau), uint16(palier));
                        }
                    }
                }
            }


            //reputations
            for (int i = 0; i<6 ; i++) {
                uint16 niveau = fields[22+2*i].GetInt16();
                uint16 palier = fields[22+2*i+1].GetInt16();

                niveau += palier;
                if (niveau<1) niveau = 1;
                if (niveau>42999) niveau = 42999;

                uint16 reputid;
                switch (i) {
                case 0: //gardiens hyjal
                    reputid = 1158;
                    break;
                case 1: //cercle terrestre
                    reputid = 1135;
                    break;
                case 2: //ramkahen
                    reputid = 1173;
                    break;
                case 3: //therazane
                    reputid = 1171;
                    break;
                case 4: //gardiens baradin (alliance)
                    if (!player->GetTeamId() == TEAM_ALLIANCE) continue;
                    reputid = 1177;
                    break;
                case 5: //poing hurlenfer (horde)
                    if (!player->GetTeamId() == TEAM_HORDE) continue;
                    reputid = 1178;
                    break;
                default:
                    continue;
                }

                FactionEntry const *factionEntry = sFactionStore.LookupEntry(reputid);
                if (factionEntry && factionEntry->reputationListID>=0)
                    player->SetReputation(reputid,niveau);
            }


            //monnaie
            uint32 monnaie = fields[38].GetInt32();
            if (monnaie>=0)
                player->ModifyMoney(player->GetMoney()+monnaie);

            //sacs
            int isac = 0;
            for (uint8 ibag = INVENTORY_SLOT_BAG_START; ibag < INVENTORY_SLOT_BAG_END; ibag++) {
                uint32 sacid = fields[34+isac].GetUInt32();
                if (isac==0 && sacid == 0) sacid = 4498;

                isac++;
                if (sacid != 0) {
                    if (Bag *pBag = (Bag*)player->GetItemByPos(INVENTORY_SLOT_BAG_0, ibag)) {

                        for (uint32 jsize = 0; jsize < pBag->GetBagSize(); jsize++){
                            if (pBag->GetItemByPos(jsize)){
                                player->DestroyItem(ibag, jsize, true);
                            }
                        }
                        if (pBag->IsEmpty()) {
                            player->DestroyItem(INVENTORY_SLOT_BAG_0, ibag, true);
                        } else continue;
                    }

                    ItemPrototype const *pProto = ObjectMgr::GetItemPrototype(sacid);
                    if (pProto && pProto->InventoryType == INVTYPE_BAG) {
                        Item *sac = Item::CreateItem(sacid, 1);
                        player->EquipItem(ibag, sac, true);
                    }
                }
            }

            //equipement
            uint32 stuff = fields[5].GetUInt32();
            uint32 pvepvp = fields[4].GetUInt32();

            const char* reqniveau = "SELECT niveau, count(*) FROM site_recuperations_cata_stuff WHERE classe='%u' AND niveau <='%u' GROUP BY niveau ORDER BY niveau DESC LIMIT 1;";
            QueryResult resultniveau = SiteDatabase.PQuery(reqniveau, player->getClass(), niveauattrib);
            if (resultniveau) {
                Field* fieldsniveau = resultniveau->Fetch();

                if (fieldsniveau[1].GetUInt32() != 0) {
                    uint32 niveaupallier = fieldsniveau[0].GetUInt32();

                    QueryResult resultstuff;
                    Field* fieldstuff;
                    uint32 reqstufftype = 0;

                    bool stufffound = false;
                    //test avec tous les choix du joueur
                    if (!stufffound) {
                        const char* reqstuff = "SELECT count(*) FROM site_recuperations_cata_stuff WHERE classe='%u' AND stuff='%u' AND alliancehorde='%u' AND pvepvp='%u' AND niveau ='%u' ;";
                        resultstuff = SiteDatabase.PQuery(reqstuff, player->getClass(), stuff, player->GetTeamId()+1, pvepvp, niveaupallier);
                        if (resultstuff) {
                            fieldstuff = resultstuff->Fetch();
                            if (fieldstuff[0].GetUInt32() != 0) {
                                reqstufftype = 1;
                                stufffound = true;
                            }
                        }
                    }

                    //test en pve
                    if (!stufffound) {
                        const char* reqstuff = "SELECT count(*) FROM site_recuperations_cata_stuff WHERE classe='%u' AND stuff='%u' AND alliancehorde='%u' AND pvepvp='%u' AND niveau ='%u' ;";
                        resultstuff = SiteDatabase.PQuery(reqstuff, player->getClass(), stuff, player->GetTeamId()+1, 0, niveaupallier);
                        if (resultstuff) {
                            fieldstuff = resultstuff->Fetch();
                            if (fieldstuff[0].GetUInt32() != 0) {
                                reqstufftype = 2;
                                stufffound = true;
                            }
                        }
                    }

                    //test en pve neutre
                    if (!stufffound) {
                        const char* reqstuff = "SELECT count(*) FROM site_recuperations_cata_stuff WHERE classe='%u' AND stuff='%u' AND alliancehorde='%u' AND pvepvp='%u' AND niveau ='%u' ;";
                        resultstuff = SiteDatabase.PQuery(reqstuff, player->getClass(), stuff, 0, 0, niveaupallier);
                        if (resultstuff) {
                            fieldstuff = resultstuff->Fetch();
                            if (fieldstuff[0].GetUInt32() != 0) {
                                reqstufftype = 3;
                                stufffound = true;
                            }
                        }
                    }

                    //test en pve dps alli/horde
                    if (!stufffound) {
                        const char* reqstuff = "SELECT count(*) FROM site_recuperations_cata_stuff WHERE classe='%u' AND stuff='%u' AND alliancehorde='%u' AND pvepvp='%u' AND niveau ='%u' ;";
                        resultstuff = SiteDatabase.PQuery(reqstuff, player->getClass(), 0, player->GetTeamId()+1, 0, niveaupallier);
                        if (resultstuff) {
                            fieldstuff = resultstuff->Fetch();
                            if (fieldstuff[0].GetUInt32() != 0) {
                                reqstufftype = 4;
                                stufffound = true;
                            }
                        }
                    }

                    //test en pve neutre dps
                    if (!stufffound) {
                        const char* reqstuff = "SELECT count(*) FROM site_recuperations_cata_stuff WHERE classe='%u' AND stuff='%u' AND alliancehorde='%u' AND pvepvp='%u' AND niveau ='%u' ;";
                        resultstuff = SiteDatabase.PQuery(reqstuff, player->getClass(), 0, 0, 0, niveaupallier);
                        if (resultstuff) {
                            fieldstuff = resultstuff->Fetch();
                            if (fieldstuff[0].GetUInt32() != 0) {
                                reqstufftype = 5;
                                stufffound = true;
                            }
                        }
                    }

                    if (reqstufftype!=0) {

                        char* reqstuffattrib = "SELECT stuff_id FROM site_recuperations_cata_stuff WHERE classe='%u' AND stuff='%u' AND alliancehorde='%u' AND pvepvp='%u' AND niveau ='%u' ;";
                        QueryResult resultstuffattrib;


                        switch (reqstufftype) {
                        case 1:
                            resultstuffattrib = SiteDatabase.PQuery(reqstuffattrib, player->getClass(), stuff, player->GetTeamId()+1, pvepvp, niveaupallier);
                            break;
                        case 2:
                            resultstuffattrib = SiteDatabase.PQuery(reqstuffattrib, player->getClass(), stuff, player->GetTeamId()+1, 0, niveaupallier);
                            break;
                        case 3:
                            resultstuffattrib = SiteDatabase.PQuery(reqstuffattrib, player->getClass(), stuff, 0, 0, niveaupallier);
                            break;
                        case 4:
                            resultstuffattrib = SiteDatabase.PQuery(reqstuffattrib, player->getClass(), 0, player->GetTeamId()+1, 0, niveaupallier);
                            break;
                        case 5:
                            resultstuffattrib = SiteDatabase.PQuery(reqstuffattrib, player->getClass(), 0, 0, 0, niveaupallier);
                            break;
                        default:
                            break;
                        }

                        if(!resultstuffattrib)
                            return false;
                        uint32 bagpos = INVENTORY_SLOT_ITEM_START;
                        uint32 bag = INVENTORY_SLOT_BAG_0;

                        do {
                            Field* fieldstuffid = resultstuffattrib->Fetch();
                            uint32 stuffid = fieldstuffid[0].GetUInt32();

                            ItemPrototype const *pProto = ObjectMgr::GetItemPrototype(stuffid);

                            if (pProto) {
                                if (bagpos == INVENTORY_SLOT_ITEM_END) {
                                    bagpos = 0;
                                    bag = INVENTORY_SLOT_BAG_START;
                                }

                                player->DestroyItem(bag, bagpos, true);
                                player->AddItem(stuffid, 1);

                                bagpos++;
                            }


                        } while (resultstuffattrib->NextRow());
                    }
                }
            }

        }*/

		
		/*
        Player *pPlayer = handler->GetSession()->GetPlayer();

        if(!pPlayer)
            return false;

        uint32 guidLow = pPlayer->GetGUIDLow();

	//sLog->outDebug(LOG_FILTER_NETWORKIO, "NOBODIE test1");

        QueryResult result = LoginDatabase.PQuery("SELECT id, level, stuff_wanted FROM recuperation WHERE idPerso=%u AND state=3", guidLow);
        if(!result)
        {
            handler->PSendSysMessage("Vous ne disposez pas de recuperation pour ce personnage, ou elle n'a pas encore ete validee par un maitre du jeu");
            return true;
        }

	//sLog->outDebug(LOG_FILTER_NETWORKIO, "NOBODIE test2");

        Field *recup = result->Fetch();

        uint32 id = recup[0].GetUInt32();
        uint32 stuffId = recup[2].GetUInt32();
        uint8 level = recup[1].GetUInt8();

        QueryResult metier = LoginDatabase.PQuery("SELECT skill, value, max FROM recuperation_metier WHERE idRecup = %u", id);
        QueryResult reput = LoginDatabase.PQuery("SELECT reputId, value FROM recuperation_reput WHERE idRecup = %u", id);
        QueryResult stuff = LoginDatabase.PQuery("SELECT itemId FROM recuperation_stuff WHERE spe = %u", stuffId);


        uint32 sacId = 0;
        uint32 moneyToAdd = 0;
        if(level == 85)
        {
            sacId = 21841;
            moneyToAdd = 12000 * GOLD;
        }
        else
        {
            sacId = 30744;
            moneyToAdd = 10000 * GOLD;
        }

	//sLog->outDebug(LOG_FILTER_NETWORKIO, "NOBODIE test3");

        pPlayer->ModifyMoney(moneyToAdd);

        for(uint32 i = INVENTORY_SLOT_BAG_START ; i < INVENTORY_SLOT_BAG_END ; i++)
        {
            if(Bag *pBag = (Bag*)pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            {
                for(uint32 j = 0 ; j < pBag->GetBagSize(); j++)
                {
                    if(Item *pItem = pBag->GetItemByPos(j))
                        pPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, j, false);
                }

                pPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, i, true);
            }

        }

	//sLog->outDebug(LOG_FILTER_NETWORKIO, "NOBODIE test4");


        for(uint32 i = INVENTORY_SLOT_ITEM_START ; i < INVENTORY_SLOT_ITEM_END ; i++)
        {
            if(pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                pPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, i, true);
        }

	//sLog->outDebug(LOG_FILTER_NETWORKIO, "NOBODIE test5");


        for(uint32 i = 0 ; i < 4 ; i++)
        {
            pPlayer->EquipNewItem(INVENTORY_SLOT_BAG_START+i, sacId, true);
        }

	//sLog->outDebug(LOG_FILTER_NETWORKIO, "NOBODIE test6");

        pPlayer->GiveLevel(level);
        pPlayer->InitTalentForLevel();
        pPlayer->SetUInt32Value(PLAYER_XP,0);

        if(stuff)
        {
            do
            {
                Field * piece = stuff->Fetch();
                uint32 item = piece[0].GetUInt32();
                pPlayer->AddItem(item, 1);
            }
            while(stuff->NextRow());
        }

	//sLog->outDebug(LOG_FILTER_NETWORKIO, "NOBODIE test7");

        if(reput)
        {
            do
            {
                Field *rep = reput->Fetch();
                uint32 reputId = rep[0].GetUInt32();
                uint32 rank = rep[1].GetUInt32();

                FactionEntry const *fac = sFactionStore.LookupEntry(reputId);

                if(rank == 42000)
                    rank += 1000;

                pPlayer->GetReputationMgr().SetReputation(fac, rank);
            }
            while(reput->NextRow());
        }

	//sLog->outDebug(LOG_FILTER_NETWORKIO, "NOBODIE test8");

        if(metier)
        {
            do
            {
                Field *prof = metier->Fetch();

                uint32 skill = prof[0].GetUInt32();
                uint32 value = prof[1].GetUInt32();
                uint32 max = prof[2].GetUInt32();

                QueryResult profSpellQ = LoginDatabase.PQuery("SELECT competence FROM recuperation_metier_spell WHERE id = %u AND niveau = %u", skill, max);
                if(profSpellQ->GetRowCount() == 0)
                    continue;

                Field *profSpell = profSpellQ->Fetch();
                uint32 spell = profSpell->GetUInt32();

                if(!pPlayer->HasSpell(spell))
                    pPlayer->learnSpell(spell, false);

                pPlayer->SetSkill(skill, pPlayer->GetSkillStep(skill), value, max);
            }
            while(metier->NextRow());
        }

	//sLog->outDebug(LOG_FILTER_NETWORKIO, "NOBODIE test9");


        LoginDatabase.PQuery("UPDATE recuperation SET state=5 WHERE id=%u", id);

	//sLog->outDebug(LOG_FILTER_NETWORKIO, "NOBODIE test10");

        if (pPlayer->getRace() == RACE_GOBLIN)
            pPlayer->TeleportTo(1, 1569, -4398, 18, 0);
        else if (pPlayer->getRace() == RACE_WORGEN)
            pPlayer->TeleportTo(0, -8830, 626, 96, 0);

        pPlayer->SaveToDB();

	//sLog->outDebug(LOG_FILTER_NETWORKIO, "NOBODIE test11");


        return true;*/

        Player *pPlayer = handler->GetSession()->GetPlayer();

        if(!pPlayer)
            return false;

        uint32 guidLow = pPlayer->GetGUIDLow();

        QueryResult result = LoginDatabase.PQuery("SELECT id, level, stuff_wanted FROM recuperation WHERE idPerso=%u AND state=3 AND destination_realm=%u", guidLow, realmID);
        if(!result)
        {
            handler->PSendSysMessage("Vous ne disposez pas de recuperation pour ce personnage, ou elle n'a pas encore été validee par un maitre du jeu");
            return true;
        }

        Field *recup = result->Fetch();

        uint32 id = recup[0].GetUInt32();
        uint32 stuffId = recup[2].GetUInt32();
        uint8 level = recup[1].GetUInt8();

        QueryResult metier = LoginDatabase.PQuery("SELECT skill, value, max FROM recuperation_metier WHERE idRecup = %u", id);
        QueryResult reput = LoginDatabase.PQuery("SELECT reputId, value FROM recuperation_reput WHERE idRecup = %u", id);
        QueryResult stuff = LoginDatabase.PQuery("SELECT itemId FROM recuperation_stuff WHERE spe = %u", stuffId);


        uint32 sacId = 0;
        uint32 moneyToAdd = 0;
        if(level == 85)
        {
            sacId = 21841;
            moneyToAdd = 12000 * GOLD;
        }
        else
        {
            sacId = 30744;
            moneyToAdd = 10000 * GOLD;
        }

        pPlayer->ModifyMoney(moneyToAdd);

        for(uint32 i = INVENTORY_SLOT_BAG_START ; i < INVENTORY_SLOT_BAG_END ; i++)
        {
            if(Bag *pBag = (Bag*)pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            {
                for(uint32 j = 0 ; j < pBag->GetBagSize(); j++)
                {
                    if(Item *pItem = pBag->GetItemByPos(j))
                        pPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, j, false);
                }

                pPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, i, true);
            }

        }

        for(uint32 i = INVENTORY_SLOT_ITEM_START ; i < INVENTORY_SLOT_ITEM_END ; i++)
        {
            if(pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                pPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, i, true);
        }

        for(uint32 i = 0 ; i < 4 ; i++)
        {
            pPlayer->EquipNewItem(INVENTORY_SLOT_BAG_START+i, sacId, true);
        }

        pPlayer->GiveLevel(level);
        pPlayer->InitTalentForLevel();
        pPlayer->SetUInt32Value(PLAYER_XP,0);

        if(stuff)
        {
            do
            {
                Field * piece = stuff->Fetch();
                uint32 item = piece[0].GetUInt32();
                pPlayer->AddItem(item, 1);
            }
            while(stuff->NextRow());
        }

        if(reput)
        {
            do
            {
                Field *rep = reput->Fetch();
                uint32 reputId = rep[0].GetUInt32();
                uint32 rank = rep[1].GetUInt32();

                FactionEntry const *fac = sFactionStore.LookupEntry(reputId);

                if(rank == 42000)
                    rank += 1000;

                pPlayer->GetReputationMgr().SetReputation(fac, rank);
            }
            while(reput->NextRow());
        }

        if(metier)
        {
            do
            {
                Field *prof = metier->Fetch();

                uint32 skill = prof[0].GetUInt32();
                uint32 value = prof[1].GetUInt32();
                uint32 max = prof[2].GetUInt32();

                QueryResult profSpellQ = LoginDatabase.PQuery("SELECT competence FROM recuperation_metier_spell WHERE id = %u AND niveau = %u", skill, max);
                if(profSpellQ->GetRowCount() == 0)
                    continue;

                Field *profSpell = profSpellQ->Fetch();
                uint32 spell = profSpell->GetUInt32();

                if(!pPlayer->HasSpell(spell))
                    pPlayer->learnSpell(spell, true);

                pPlayer->SetSkill(skill, pPlayer->GetSkillStep(skill), value, max);
            }
            while(metier->NextRow());
        }

        LoginDatabase.PQuery("UPDATE recuperation SET state=5 WHERE id=%u", id);

        if (pPlayer->getRace() == RACE_GOBLIN)
            pPlayer->TeleportTo(1, 1569, -4398, 18, 0);
        else if (pPlayer->getRace() == RACE_WORGEN)
            pPlayer->TeleportTo(0, -8830, 626, 96, 0);

        pPlayer->SaveToDB();

        return true;
    }


    static bool HandleBoutiqueRaceCommand(ChatHandler *handler, const char *args) {
        Player* target = handler->GetSession()->GetPlayer();

        const char* reqcount = "SELECT id FROM boutique_service_achat WHERE accountId='%u' AND type=3 AND recup=0 LIMIT 1";

        sLog->outDebug(LOG_FILTER_NETWORKIO, reqcount, handler->GetSession()->GetAccountId());
        QueryResult resultcount = LoginDatabase.PQuery(reqcount, handler->GetSession()->GetAccountId());

        if (!resultcount) {
            //Vous ne disposez actuellement d'aucun service de ce type. Vous pouvez acheter ce service sur la boutique
            handler->PSendSysMessage(11016);
            return true;
        }

        Field* fieldscount = resultcount->Fetch();

        const char* qmask = "SELECT 1 FROM boutique_service WHERE realmMask & '%u' != 0 and type = 3";
        QueryResult reqmask = LoginDatabase.PQuery(qmask, (1<<(realmID-1)));
        if (!reqmask) {
            //Ce produit ou service n'est pas disponible pour ce royaume.
            handler->PSendSysMessage(11018);
            return true;
        }
		
		
        if (target)
        {
            //Le changement de race est valide. Vous pourrez effectuer le changement sur l'ecran des personnages
            handler->PSendSysMessage(12009);
            target->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
            CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '128' WHERE guid = %u", target->GetGUIDLow());

            target->SaveToDB();
            const char* requpdate = "UPDATE boutique_service_achat SET realmID = '%u', recup='%u' WHERE id='%u'";
            LoginDatabase.PExecute(requpdate, realmID, (uint32)handler->GetSession()->GetPlayer()->GetGUID(), fieldscount[0].GetInt32());
        }

        return true;
    }

    static bool HandleBoutiqueFactionCommand(ChatHandler *handler, const char *args) {
        Player* target = handler->GetSession()->GetPlayer();

        const char* reqcount = "SELECT id FROM boutique_service_achat WHERE accountId='%u' AND type=4 AND recup=0 LIMIT 1";

        sLog->outDebug(LOG_FILTER_NETWORKIO, reqcount, handler->GetSession()->GetAccountId());
        QueryResult resultcount = LoginDatabase.PQuery(reqcount, handler->GetSession()->GetAccountId());        
        if (!resultcount) {
            //Vous ne disposez actuellement d'aucun service de ce type. Vous pouvez acheter ce service sur la boutique
            handler->PSendSysMessage(11016);
            return true;
        }

        Field* fieldscount = resultcount->Fetch();
		
        const char* qmask = "SELECT 1 FROM boutique_service WHERE realmMask & '%u' != 0 and type = 4";
        QueryResult reqmask = LoginDatabase.PQuery(qmask, (1<<(realmID-1)));        
        if (!reqmask) {
            //Ce produit ou service n'est pas disponible pour ce royaume.
            handler->PSendSysMessage(11018);
            return true;
        }
		
        const char* reqallowed = "SELECT value FROM data WHERE RealmID = %u AND identifier = 'lowestFaction';";
        sLog->outDebug(LOG_FILTER_NETWORKIO, reqallowed, realmID);
        QueryResult resultallowed = LoginDatabase.PQuery(reqallowed, realmID);
        Field* fieldsallowed = resultallowed->Fetch();
        if (fieldsallowed[0].GetInt32()==target->GetTeamId()) {
            //Changement impossible vers l'autre faction car trop peuplÃ©e
            handler->PSendSysMessage(12007);
            return true;
        }

        if (target)
        {
            //Le changement de faction est valide. Vous pourrez effectuer le changement sur l'ecran des personnages
            handler->PSendSysMessage(12008);
            target->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
            CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '64' WHERE guid = '%u'", target->GetGUIDLow());

            target->SaveToDB();
            const char* requpdate = "UPDATE boutique_service_achat SET realmID = '%u', recup='%u' WHERE id='%u'";
            LoginDatabase.PExecute(requpdate, realmID, (uint32)handler->GetSession()->GetPlayer()->GetGUID(), fieldscount[0].GetInt32());
        }

        return true;
    }

};

void AddSc_boutique()
{
    new boutique_cs;
}
