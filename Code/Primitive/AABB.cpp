#include "AABB.h"

#include "Novice.h"

bool AABB::Collision(const AABB &a,const AABB &b) {
    if(b.min.x <= a.max.x && b.max.x >= a.min.x &&
       b.min.y <= a.max.y && b.max.y >= a.min.y &&
       b.min.z <= a.max.z && b.max.z >= a.min.z) {
        return true;
    }
    return false;
}

void AABB::Draw(const MyMatrix4x4 &viewProjectionMa,const MyMatrix4x4 &viewPortMa) {
    Vec3 vertices[8];
    vertices[0] = {min.x,min.y,min.z};
    vertices[1] = {max.x,min.y,min.z};
    vertices[2] = {min.x,max.y,min.z};
    vertices[3] = {max.x,max.y,min.z};
    vertices[4] = {min.x,min.y,max.z};
    vertices[5] = {max.x,min.y,max.z};
    vertices[6] = {min.x,max.y,max.z};
    vertices[7] = {max.x,max.y,max.z};

    // 頂点をスクリーン座標に変換
    for(int i = 0; i < 8; ++i) {
        vertices[i] = TransformVector(TransformVector(vertices[i], viewProjectionMa),viewPortMa);
    }

    // 12本の線を描画（重複はしない）
    Novice::DrawLine(static_cast<int>(vertices[0].x),static_cast<int>(vertices[0].y),static_cast<int>(vertices[1].x),static_cast<int>(vertices[1].y),color); // min to (max.x, min.y, min.z)
    Novice::DrawLine(static_cast<int>(vertices[1].x),static_cast<int>(vertices[1].y),static_cast<int>(vertices[3].x),static_cast<int>(vertices[3].y),color); // (max.x, min.y, min.z) to (max.x, max.y, min.z)
    Novice::DrawLine(static_cast<int>(vertices[3].x),static_cast<int>(vertices[3].y),static_cast<int>(vertices[2].x),static_cast<int>(vertices[2].y),color); // (max.x, max.y, min.z) to (min.x, max.y, min.z)
    Novice::DrawLine(static_cast<int>(vertices[2].x),static_cast<int>(vertices[2].y),static_cast<int>(vertices[0].x),static_cast<int>(vertices[0].y),color); // (min.x, max.y, min.z) to min

    Novice::DrawLine(static_cast<int>(vertices[4].x),static_cast<int>(vertices[4].y),static_cast<int>(vertices[5].x),static_cast<int>(vertices[5].y),color); // (min.x, min.y, max.z) to (max.x, min.y, max.z)
    Novice::DrawLine(static_cast<int>(vertices[5].x),static_cast<int>(vertices[5].y),static_cast<int>(vertices[7].x),static_cast<int>(vertices[7].y),color); // (max.x, min.y, max.z) to (max.x, max.y, max.z)
    Novice::DrawLine(static_cast<int>(vertices[7].x),static_cast<int>(vertices[7].y),static_cast<int>(vertices[6].x),static_cast<int>(vertices[6].y),color); // (max.x, max.y, max.z) to (min.x, max.y, max.z)
    Novice::DrawLine(static_cast<int>(vertices[6].x),static_cast<int>(vertices[6].y),static_cast<int>(vertices[4].x),static_cast<int>(vertices[4].y),color); // (min.x, max.y, max.z) to (min.x, min.y, max.z)

    Novice::DrawLine(static_cast<int>(vertices[0].x),static_cast<int>(vertices[0].y),static_cast<int>(vertices[4].x),static_cast<int>(vertices[4].y),color); // min to (min.x, min.y, max.z)
    Novice::DrawLine(static_cast<int>(vertices[1].x),static_cast<int>(vertices[1].y),static_cast<int>(vertices[5].x),static_cast<int>(vertices[5].y),color); // (max.x, min.y, min.z) to (max.x, min.y, max.z)
    Novice::DrawLine(static_cast<int>(vertices[2].x),static_cast<int>(vertices[2].y),static_cast<int>(vertices[6].x),static_cast<int>(vertices[6].y),color); // (min.x, max.y, min.z) to (min.x, max.y, max.z)
    Novice::DrawLine(static_cast<int>(vertices[3].x),static_cast<int>(vertices[3].y),static_cast<int>(vertices[7].x),static_cast<int>(vertices[7].y),color); // (max.x, max.y, min.z) to (max.x, max.y, max.z)
}
