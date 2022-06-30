#include "Scene.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "IndexBuffer.h"
#include "AssimpLoader.h"
#include "DescriptorHeap.h"
#include "Texture2D.h"

std::vector<std::unique_ptr<VertexBuffer>>		vertexBuffer;
std::vector<std::unique_ptr<IndexBuffer>>		indexBuffer;
std::vector<std::unique_ptr<ConstantBuffer>>	constantBuffer;
std::unique_ptr<RootSignature>					rootSignature;
std::unique_ptr<PipelineState>					pipelineState;
std::vector<Mesh>								meshes;
std::unique_ptr<DescriptorHeap>					descriptorHeap;
std::vector<std::unique_ptr<Texture2D>>			pTexs;

const wchar_t* modelFile = L"Assets/Alicia/FBX/Alicia_solid_Unity.FBX";

//初期化処理
bool Scene::Init() {

	ImportSettings importSetting = { modelFile,meshes,false,true };

	AssimpLoader loder;
	if (!loder.Load(importSetting)) {
		return false;
	}
	

	//VertexBuffer Init
	vertexBuffer.reserve(meshes.size());
	for (size_t i = 0; i < meshes.size(); ++i) {
		auto size = sizeof(Vertex) * meshes[i].Vertices.size();
		auto stride = sizeof(Vertex);
		auto vertices = meshes[i].Vertices.data();
		
		std::unique_ptr<VertexBuffer> pVB;
		pVB.reset(new VertexBuffer(size, stride, vertices, gEngine));

		if (!pVB->IsValid()) {
			ErrorMessage(gEngine->GetWnd()->getHandleWnd(), TEXT("頂点バッファの生成に失敗"));
			return false;
		}

		vertexBuffer.push_back(std::move(pVB));
	}
	

	//indexBuffer Init
	indexBuffer.reserve(meshes.size());

	for (size_t i = 0; i < meshes.size(); ++i) {
		auto size		= sizeof(uint32_t) * meshes[i].Indices.size();
		auto indices	= meshes[i].Indices.data();

		std::unique_ptr<IndexBuffer> pIB;
		pIB.reset(new IndexBuffer(size, indices, gEngine));

		if (!pIB->IsValid()) {
			ErrorMessage(gEngine->GetWnd()->getHandleWnd(), TEXT("インデックスバッファの生成に失敗"));
			return false;
		}

		indexBuffer.push_back(std::move(pIB));
	}

	//ConstantBuffer Init
	auto eyePos		= DirectX::XMVectorSet(0.0f, 120.0f, 75.0f, 0.0f);
	auto targetPos	= DirectX::XMVectorSet(0.0f, 120.f, 0.0f, 0.0f);
	auto upward		= DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	auto fov		= DirectX::XMConvertToRadians(60);
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

	descriptorHeap.reset(new DescriptorHeap(gEngine));
	for (size_t i = 0; i < meshes.size(); ++i) {
		auto texPath = ReplaceExtension(meshes[i].DiffuseMap, "tga");
		auto mainTex = Texture2D::Get(texPath, gEngine);
		std::unique_ptr<Texture2D> pTex(mainTex);
		descriptorHeap->Register(pTex.get());
		pTexs.push_back(std::move(pTex));
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
	auto materialHeap	= descriptorHeap->GetHeap();

	for (size_t i = 0; i < meshes.size(); ++i) {
		auto vbView	= vertexBuffer[i]->View();
		auto ibView	= indexBuffer[i]->View();

		commndList->SetGraphicsRootSignature(rootSignature->Get());
		commndList->SetPipelineState(pipelineState->Get());
		commndList->SetGraphicsRootConstantBufferView(0, constantBuffer[currentIndex]->GetAddress());

		commndList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commndList->IASetVertexBuffers(0, 1, &vbView);
		commndList->IASetIndexBuffer(&ibView);

		commndList->SetDescriptorHeaps(1, &materialHeap);
		commndList->SetGraphicsRootDescriptorTable(1, descriptorHeap->pHandles[i]->HandleGPU);

		commndList->DrawIndexedInstanced(meshes[i].Indices.size(), 1, 0, 0, 0);
	}
	
}
