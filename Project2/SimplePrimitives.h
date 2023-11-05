#pragma once

bool GetSimpleCube(ID3D11Device* _d3dDevice,
				   ID3D11DeviceContext* _d3dImmediateContext,
				   ID3D11VertexShader** _d3dVS,
				   ID3D11Buffer* _VertexBuff,
				   ID3D11InputLayout* _Layout,
				   ID3D11Buffer* _IndexBuff, 
				   const wchar_t* _texFile, 
				   ID3D11ShaderResourceView** _texRV, 
				   ID3D11SamplerState** _SamplerLinear);
