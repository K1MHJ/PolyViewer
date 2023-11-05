#pragma once

#include "logGL.hpp"
#include "Geometry.hpp"
#include <vector>
#include <iostream>
#include <list>
#include <memory>
#include <format>
#include "InRangeTree.hpp"
#include "Render.hpp"
#include <random>

using namespace Geometry;

class CCanvas
{
    const float MIN_X = -2000.0f;
    const float MAX_X =  2000.0f;
    const float MIN_Y = -2000.0f;
    const float MAX_Y =  2000.0f;
    struct ObjectPosPair{
        GObject * pObj;
        float X;
        float Y;
        uint8_t Layer;
    };
    std::vector<GObject*> objects_stock;
    std::vector<ObjectPosPair> objects_arrange;
    CInRangeTreeManager<ObjectPosPair> ObjTree;
    std::vector<OBJECT_FOR_TREE<ObjectPosPair>*> ObjItem;
    glm::mat4 m_mvp;
public:
    CCanvas(){

        // 一様実数分布
        //std::uniform_real_distribution<float> dist1(-1.0f, 1.0f);
        // 正規分布
        // 平均1.0f、標準偏差0.5fで分布させる
        //std::normal_distribution<float> dist2(1.0f, 0.5f);
    }
    ~CCanvas(){

    }
    void Open(){
        std::random_device seed_gen;
        std::mt19937 engine(seed_gen());
        std::uniform_real_distribution<float> dist_w(5.0f, 20.0f);
        std::uniform_real_distribution<float> dist_h(5.0f, 20.0f);
        //std::uniform_real_distribution<float> dist_x(MIN_X, MAX_X);
        //std::uniform_real_distribution<float> dist_y(MIN_Y, MAX_Y);
        std::normal_distribution<float> dist_x(0.0f, 500.0f);
        std::normal_distribution<float> dist_y(0.0f, 500.0f);
        for(int i = 0;i<1000;i++){
            DrawBox(dist_x(engine), dist_y(engine), dist_w(engine), dist_h(engine));
        }

        // std::vector<GPoint> pos;
        // for(int i = 0;i<4;i++){
        //     pos.push_back(GPoint(i * 5.0f, i * 5.0f));
        // }
        // GBox *box = new GBox(-5.0f, -5.0f, 5.0f, 5.0f);
        // ArrangeBox(pos, box);

        ObjTree.Init(5, MIN_X, MIN_Y, MAX_X, MAX_Y);
        for(auto& objPos : objects_arrange)
        {
            OBJECT_FOR_TREE<ObjectPosPair> *oft = new OBJECT_FOR_TREE<ObjectPosPair>();
            oft->m_pObject = &objPos;
            const GType::en type = objPos.pObj->Type();
            if(type == GType::BOX){
                GBox* b = (GBox*)(objPos.pObj);
                float left = b->botLeft.X + objPos.X;
                float bottom = b->botLeft.Y + objPos.Y;
                float right = b->topRight.X + objPos.X;
                float top = b->topRight.Y + objPos.Y;
                ObjItem.push_back(oft);
                ObjTree.Regist(left, bottom, right, top, oft);
            }
        }
        // {
        //     InRangeList<ObjectPosPair> *inObjs;
        //     DWORD num = ObjTree.GetAllInRangeObjs(3, 3, 6, 6, &inObjs);
        //     auto root = inObjs->getRootPtr();
        //     for(int i = 0;i<num;i++){
        //         ObjectPosPair* p = root[i];
        //         const GType::en type = p->pObj->Type();
        //         if(type == GType::BOX){
        //             GBox* b = (GBox*)p->pObj;
        //             std::cout <<"("<< b->botLeft.X << "," << b->botLeft.Y << ")"
        //             << b->topRight.X << "," << b->topRight.Y 
        //             << "[" << p->X << "," << p->Y << "]"
        //             << std::endl;
        //         }
        //     }
        // }
    }
    void Render(int wnd_w_pxl, int wnd_h_pxl, float wnd_x0, float wnd_y0, float pxl_per_um){
        glViewport(0, 0, wnd_w_pxl, wnd_h_pxl);
        glm::mat4 projection = glm::ortho(
            wnd_x0, 
            wnd_x0 + wnd_w_pxl / pxl_per_um,
            wnd_y0,
            wnd_y0 + wnd_h_pxl/ pxl_per_um);
        glm::mat4 view       = glm::lookAt(
            glm::vec3(0,0,1),
            glm::vec3(0,0,0),
            glm::vec3(0,1,0)
        );
        m_mvp = projection * view;
        InRangeList<CCell<ObjectPosPair>> *inCells;
        DWORD num = ObjTree.GetAllInRangeCells(
            wnd_x0, wnd_y0, 
            wnd_x0 + wnd_w_pxl / pxl_per_um, wnd_y0 + wnd_h_pxl / pxl_per_um,
            &inCells);
        if(num == 0) return;
        CCell<ObjectPosPair>** root = inCells->getRootPtr();
        for(int i = 0;i<num;i++){
            if(root[i]->m_width * pxl_per_um <= 3.0){
                //draw dot
            }else{
                OBJECT_FOR_TREE<ObjectPosPair>* spOFT = root[i]->GetFirstObj();
                while( spOFT != NULL )
                {
                    ObjectPosPair* pObj = spOFT->m_pObject;
                    Render(pObj, m_mvp);
                    spOFT = spOFT->m_spNext;
                }
            }
        }
    }
    void Render(ObjectPosPair* pobj, const glm::mat4 & mvp){
        const GType::en& type = pobj->pObj->Type();
        if(type == GType::BOX){
            GBox* pBox = (GBox*)(pobj->pObj);
            GL::RenderGObject(pBox, pobj->X, pobj->Y, mvp);
        }
    }
    void DrawBox(float left, float bot, float width, float height){
        ObjectPosPair po;
        po.pObj = new GBox(GPoint(0,0), GPoint(width, height));
        po.X = left;
        po.Y = bot;
        objects_stock.push_back(po.pObj);
        objects_arrange.push_back(po);
    }
    void ArrangeBox(const std::vector<GPoint>& pos, GBox *box){
        objects_stock.push_back(box);
        for(auto it : pos){
            ObjectPosPair po;
            po.pObj = box;
            po.X = it.X;
            po.Y = it.Y;
            objects_arrange.push_back(po);
        }
    }
};