#include "Scene.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "IndexBuffer.h"

std::unique_ptr<VertexBuffer>					vertexBuffer;
std::vector<std::unique_ptr<ConstantBuffer>>	constantBuffer;
std::unique_ptr<RootSignature>					rootSignature;
std::unique_ptr<PipelineState>					pipelineState;
std::unique_ptr<IndexBuffer>					indexBuffer;

bool Scene::Init() {
	
	Vertex vertices[4] = {};
	vertices[0].Position	= DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].Color		= DirectX::XMFLOAT4( 1.0f,  0.0f, 0.0f, 1.0f);

	vertices[1].Position	= DirectX::XMFLOAT3( 1.0f,  1.0f, 0.0f);
	vertices[1].Color		= DirectX::XMFLOAT4( 0.0f,  1.0f, 0.0f, 1.0f);

	vertices[2].Position	= DirectX::XMFLOAT3( 1.0f, -1.0f, 0.0f);
	vertices[2].Color		= DirectX::XMFLOAT4( 0.0f,  0.0f, 1.0f, 1.0f);

	vertices[3].Position	= DirectX::XMFLOAT3(-1.0f,-1.0f, 0.0f);
	vertices[3].Color		= DirectX::XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f);

	//VertexBuffer Init
	vertexBuffer.reset(new VertexBuffer(sizeof(Vertex) * std::size(vertices), sizeof(Vertex), vertices, gEngine));
	if (!vertexBuffer->IsValid()) {
		ErrorMessage(gEngine->GetWnd()->getHandleWnd(), TEXT("頂点バッファの生成に失敗"));
		return false;
	}

	//indexBuffer Init
	uint32_t indices[] = { 0,1,2,0,2,3 };

	indexBuffer.reset(new IndexBuffer(sizeof(uint32_t) * std::size(indices), indices, gEngine));
	if (!indexBuffer->IsValid()) {
		ErrorMessage(gEngine->GetWnd()->getHandleWnd(), TEXT("インデックスバッファの生成に失敗"));
		return false;
	}

	//ConstantBuffer Init
	auto eyePos		= DirectX::XMVectorSet(0.0f, 0.0f, 5.0f, 0.0f);
	auto targetPos	= DirectX::XMVectorZero();
	auto upward		= DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	auto fov		= DirectX::XMConvertToRadians(37.5);
	auto aspect = static_cast<float>(gEngine->GetWnd()->GetWidth()) / static_cast<float>(gEngine->GetWnd()->GetHeight());

	constantBuffer.resize(frameCount);

	for (size_t i = 0; i < frameCount; ++i) {
		constantBuffer[i].reset(new ConstantBuffer(sizeof(Transform), gEngine));
		if (!constantBuffer[i]->IsValid()) {
			ErrorMessage(gEngine->GetWnd()->getHandleWnd(), TEXT("変換行列用定数バッファの生成に失敗"));
			return false;
		}

		auto ptr = constantBuffer[i]->GetPtr<Transform>();
		ptr->World	= DirectX::XMMatrixIdentity();
		ptr->View	= DirectX::XMMatrixLookAtRH(eyePos, targetPos, upward);
		ptr->Proj	= DirectX::XMMatrixPerspectiveFovRH(fov, aspect, 0.3f, 1000.0f);
	}

	//RootSignature Init
	rootSignature.reset(new RootSignature(gEngine));
	if (!rootSignature->IsValid()) {
		return false;
	}

	//PipelineState Init
	pipelineState.reset(new PipelineState(gEngine));
	pipelineState->SetInputLayout(Vertex::InputLayout);
	pipelineState->SetRootSignature(rootSignature->Get());
	pipelineState->SetVS(L"x64/Debug/SimpleVS.cso");
	pipelineState->SetPS(L"x64/Debug/SimplePS.cso");
	pipelineState->Create();
	if (!pipelineState->IsValid()) {
		return false;
	}


	return true;
}

//更新処理
void Scene::Update() {
	rotateY			+= 0.02f;
	auto currentIndex		= gEngine->GetFrameIndex();
	auto currentTransform	= constantBuffer[currentIndex]->GetPtr<Transform>();
	currentTransform->World = DirectX::XMMatrixRotationY(rotateY);
}

//描画処理
void Scene::Draw() {
	auto currentIndex	= gEngine->GetFrameIndex();
	auto commndList		= gEngine->GetCmdList();
	auto vbView			= vertexBuffer->View();
	auto ibView			= indexBuffer->View();

	commndList->SetGraphicsRootSignature(rootSignature->Get());
	commndList->SetPipelineState(pipelineState->Get());
	commndList->SetGraphicsRootConstantBufferView(0, constantBuffer[currentIndex]->GetAddress());

	commndList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commndList->IASetVertexBuffers(0, 1, &vbView);
	commndList->IASetIndexBuffer(&ibView);

	commndList->DrawInstanced(3, 1, 0, 0);
}
