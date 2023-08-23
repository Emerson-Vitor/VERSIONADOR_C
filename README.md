# VERSIONADOR

O presente projeto trata-se de um clone do Git, uma ferramenta amplamente utilizada para controle de versão de código. Nossa missão consiste em desenvolver um versionador funcional, totalmente implementado em linguagem C. O projeto foi idealizado e executado por um grupo composto por três dedicados alunos do curso de Ciência da Computação, no segundo semestre de sua formação acadêmica. Estamos empenhados em aprimorar nossas habilidades de programação e adquirir um profundo entendimento dos princípios fundamentais de controle de versão, enquanto aplicamos os conhecimentos adquiridos até o momento em nosso curso. Com determinação e trabalho em equipe, buscamos alcançar resultados de qualidade e contribuir para a evolução do cenário de desenvolvimento de software.


# ESTRUTURA
A estrutura de pastas da `PRIMEIRA` versão é esta a seguir:
```
versionador/
├── .versionador/
│   ├── versions.txt      // Arquivo de versões
│   └── content/          // Pasta de conteúdo dos arquivos versionados
│       ├── file1.txt
│       ├── file2.txt
│       └── ...
├── src/
│   ├── versionador.c     // Arquivo principal do programa
│   ├── database.c        // Funções para manipulação do banco de dados
│   ├── commands.c        // Funções para os comandos do versionador
│   ├── utils.c           // Funções utilitárias
│   ├── utils.h           // Cabeçalho das funções utilitárias
│   ├── commands.h        // Cabeçalho das funções dos comandos
│   └── database.h        // Cabeçalho das funções do banco de dados
└── README.md             // Documentação
```
### PARA RODAR O CÓDIGO
`tasks.json`
```json
{
	"tasks": [
		{
		"type":  "cppbuild",
		"label":  "C/C++: gcc-11 arquivo de build ativo",
		"command":  "/usr/bin/gcc-11",
		"args": [
			"-fdiagnostics-color=always",
			"-g",
			"-I${workspaceFolder}/src/utils",
			"-I${workspaceFolder}/src/database",
			"-I${workspaceFolder}/src/commands",
			"${workspaceFolder}/src/commands/*.c",
			"${workspaceFolder}/src/database/*.c",
			"${workspaceFolder}/src/utils/*.c",
			"${fileDirname}/*.c",
			"-o",
			"${fileDirname}/${fileBasenameNoExtension}"
		],
		"options": {
			"cwd":  "${fileDirname}"
		},
		"problemMatcher": [
			"$gcc"
		],
		"group": {
			"kind":  "build",
			"isDefault":  true
		},
		"detail":  "Tarefa gerada pelo Depurador."
		}
	],
	"version":  "2.0.0"
}
```

## INTEGRANTES



### <img src="https://lh3.googleusercontent.com/fife/APg5EOZOEpbjVFaEpTKNEmWzYPerLFZUqzU_E4QoOlfogAefFEp2zx57LOk0vgwkpVkp62zY9ITQUDTVwTuBeO8fslpqwr8B_K67Q1jUAu1vDP5zaiRIAZizTUCasianWnWSra8S7wKPtXkx_4jfRAN_SRr5UknYtTJIE5Ne8Y1q--69GxtJnncGsbwz6QcMNzo9GXBa_AF7Hy5qM5x_FZ5CLvTNJvy4WYoUqWsa5Ziy7Xfit8L5VucxYgGRiER7lTxIP-0-rvMqHaWBmd2wMXlyGuDKxXF5ILZ6cpi6yCu_YVaH_HzjrOqjYC2QtPzxWxIVcPbwXJTDBBJ7kFjIUbfQ_fxQ2ZihCry3wNJTPbO5SXJDzj1I1jbcIz0Yw0f97PNf76iB0fHXU96ULM7iGiIecxmpXkiButZNr9Dah--Rz4iGHQnEsRQX0Fh51hGAJcocjz7zperdrCHsLQaVWNj6qY_BHhpNHy2_wP2M000TKcL25x6ZK8vRJc24ZkJkWpbZGffnZ7e_73olf-b7bgRnHxrWrgySjInDI7j1mXx2607UEVa_2LhG8DQjSMGy0Artv1N4gzOzBPvUE_jZewVhv0BbF3QfF9npkCuEIq4wLmNPCFpXkAAjvByWEuomOgvKnPeKDcTvdW-Bt8SYyuv79KO_d17Aq9i885c2ET3bxO0d85oh_QYNrlU88IbMJp5GZlkdWy0emSPpm-NLT0z5jHTQkfiWszFb2D_Z0SMEPXc0WwM2CJLU15E8FsSqPgHaZAy7GtDyCkioO_HerKpLyzKCEnAMVkb9j7dHpYsl-qWI7x8mgqr0EITXFwmBGKLBJN9w9u4NstIjN_N6SXH3zqacVyV-h0SJuzle7mXPqv146dMCOFl9NTRKBY6Kbk9r1b-ElIM8lF9cd7Ofgd6fzycIzjHFb4BqIej1KvAULcNtLxtNLG7ctbtqhABin7L7gz0sBEdTYCywDEYzEAbe2lo7euFiSnMYzZ9hdTKij86q9RIXAffDzwmQPyT-cJbwAmkLUz3ZrZD6GamGlKkraylenPMSeHMXHmakULGvsew02M4MFDyn64ZfHy3uLQ8JzMWAZpprAOkeiYTymHS5815ry-KWfD88NSZ2vCkDkpvdFdfQEJtCQltztmkPaPUF2zLbB9o5Pv0DNJW1swpNHJG18S89aI34wo_lZr5Y5ViEOFtcZPWFdYP2GMkdbXvelMejcZSVGqzt3LymljOlab75zzDRbAojF7JKr8FqHJLM990F_s4pXNVHhGzgZ0bHtViUzQK5SCSE_2u4DhuvIEoNHEfSASCk3iIWGG23AriZFlJ_JxFg3-wWxHWC0dHQs6hG3ZW_NvFDfUjfRYsTah4mBSYfdzmqwb8SGMv9_q9VhAmQlImay9ilrymxkvY-CmB36we16mlZ9xrhsqe6B7suyHJQxO0k9jC6dwrZmgjOMYqmNcgOZNI1-YzCmKq4qSHdVO2AvmIONOx9qWdQo5y04PPDdfi26fMGhUE2XJ_998ddgvNO8MPuptlhVhULnH5C1g1bbDfF54IBOmIzwLScal7MtbcCrjB3kKha7f8um7kCNJ9vvbG4RnWoNXB4tFHKMKVlP-uGjmg=w1365-h946" height="40"> EMERSON

###  <img src="https://lh3.googleusercontent.com/fife/APg5EOax2BXI9ql9wfBfQ5rXOfyfEQh5cQT9rPx_yNo3dtxL4pCV9x2hizGCB4Dhd5WFLKw188hakgeYQU6HgHvQVa1r3XlTNNRMD47ZASn79dNVdrlBXzzRXtAYj4K4BZ6EKVY2lolbIel6kStiorw6MfPOK-CBdrMUmjCL8fJVu4M8EYA_Xspv0x9QXW1oZABn0Qxvl_CXBP3gFXvllhVcF6ykuWwIH2F34ft4KNsEYLLnVja8J6ahzSDZtN54Zuy_QgHxgzPuGWxtsoFlWTrz4Vj8htiFo8FhjNUTYu3ktqykxVz5mWl_CUs3s_EpPIKHcRMwVAxqt1FQXygM7YkvtzLXWLIW-GH9jqjmHU9zgGy__kOhRfyHB7mcsRPSScujr-dztJ_ZYH_ubBcpbNcoOwlYwSirzI31XTKe72169xNrlhbOPNsFr8Ks_XbPu7AH68B15NRCDhcb5eR2R5s8x5rMqJWgN1VLm-J9_w-oRDCnxC7_g1CxUU_7maIzqda3CI1c7S8XMX20_3xtp-LVLGGQV99zFOttc6pwLQdfEVr4AtwnuwT-wQxaCnR2D1tvb57Aat-ueqX66mpBNMgKbVMyiztlTzVlJhcQZLG444NgjngvtZcFTc9cCzIdBcL42lbtceM3ayiijbtz6iVmmKRZx_Nn1kI8xC-n5apfkvz-yfqCco37T5fhRq7X8ynDQAoHYAM_cN_WcMUXiMiri9AUWmZmUipRg7Hwh-aMboq8RII-KGzrnkyKhT5ewx3Lo3xrmOyB0yX280Y9EOzRZKWkjAFGO1NyQpR4obrflxaYAfWoomZ5TuoxYwqfq3CczFBdckF6uJF9xnwW3eBMdDbt3lzr0cfsvo7fns24YyN1g5uEjOQRjWJyj3IKPCAwFmAAnxRF9CSXN8C36x_wfa1-9tIwjZIDVnzaEgzT1XSZ4oDnG51VewMLpdKu8DQbtin3Q3TIPniC7h2E0v_wWTBZHejKyOQCkpBAbZJoxmDKJgHZEw668ytRFzKLe5-Y_IlaAyVOxCroCOTRfsgAgeN3z6u_v3HlGBMLlf_WUtF8SonQ1QOjB43QChcvy6DB-tgyu0eJhYNTg383zJVnNNNX7Ln0ZbIjAhm-vL1xlywdlA5IgOr3vdUD8wDUTBAWcWSoq0ykpWkNKm7N7uQmsfd0DWouFydZqCZHF2I5djWQNCLvREPRj8IKg6GAN3Zaxx73u8aOYzjwpGN2n90OY13ivwGtONgGHLH2r75xowiTFlK0_BST64srgMNv7GrxodeR5iedbJQcD40gj6DsF-0KSWXsIM6bcdntJjsyalbmaIHNLXfKeaoVqwNd8MjlsO4yaF9myoqWADLQ3HByxEFht9O9cckARVsXxT23ScLA855JpeBVcDA04KxonGN9dyeAqJeIVZ_mxthEM1xkALvezg9iVJcuSBtViBzyqaCA5RDFnoeTKKB_4_tGujka5PEsSnkTDg-TAAvi8KKKdWB7hDNdjxMDlRJxrLf4YpORGygx9XwxoOXu2jLHkydXj-foVHHqAPgonckBIALpGVmr6-j6E87W_2SpVvHgx9Q3WJ6gKNwhvNbHeDpwZs8SV9RCyAJygm_nCfI=w1920-h946" height="40">  MARCO 




### <img src="https://lh3.googleusercontent.com/fife/APg5EOZ55ce6RRYMC8hxYjDWcwfu1qvjq_FNX7OO0ETv1HD3Zy2WRNM2MSFsCXZ7YwCpQ11E6DzlIaYzxo1QPTDTCXQUA8IsyZLsnnlPOvwvmBCaqUn5wmB97E9LDX4MJbDlpKqyy9jZ7MxVdlJ-e99gb1CLWAHH2LpPIFhbaNCORu6-qgsZOI7BHl8SbQuS0u0bQqf9sUmsXW4H9IlVHjIZJYLUFGzqzmgzdK4p_WgERuktbcOlsyHwDbz44Zmn2zVSEVmmexcEVbQQkuK7z_AEAmAaG3_33OmIrzNKQ12FPewMbpjGZ4TGnpMHp6cKaZeJqQanKZr53eOTi5eMs0shDHQU6kxXfAPPw6Hf5GU6Y5npj7HLdc3sN9YG7B71aQWWrvbSr03i-7ueaEhzX0qaxl1yTlHSugmVIlm30Xr7LUywP0yX7i1lggKwb9H_b8olK5KEEE0WSv2RHlnkk7r19AdhCvlLfhvXKPSLmMtDv1cg74pGIobuxNDV8DusvkD2IgWc4i-aGXIkLr6TWcSql1v8hJZSFKqXPEq7tMRVRtDrQyTUrXRJtXMMXHSZCciq2RQT3HY1QdL9ve2KTwn4nbK4K5qEiOH4_W3uGAFGrYA7_gtHik2w1rXWrRKS0e8mJtiHZUOH2bYq3Wod30edApZuVZ63tI4yBsunZw57JpVUHV95XO4WVQzx3TPh8G-fCtu7vrVjonly4QDmmpOmA8JnxWpK_1mwRaSgwhZM9lqRvwVVzFFOnCWbKbgSPz_hXP3Jn_dJi7WuOfn16uIjvtRduBCbDn-l6b8idsTbGcP1Xh9VUJQgT0ImAeTrB-fhDISTQ4_rJPLZj8vArSjadPNAxfj5oSFg1GmOhRr-cjDz0Zf3za42X4q0CYAMlPSyKbo_XC-pP2wh9c56PTdfV-eUA5uk01kpxcqSFooW72bni2noP-c2_mzqeZND8nM7ARaWpGJhmt1_TD7_n49dKe2rRKqWdWXDXSI1FKw7H5xBJM5b55Kpb9w5eWw5sxu0wTrLpvQu5wRqjm-mEzMMJmpVGTLPjDQWYbqzoxkyu-TZZJmc7AyLu2mAVal7aeWBFmp18AD4KJZrOgjZSjRp6IL6GgAvPlBIpr_ePRpJzqJ-iqP5pItV2DE4RsgZwF4JJeJxz4fKq1inp9MU6aHINiFzh3biCfwu9ZoHcuOZw-WnHi3dOu_pJmnnM1SMQISzh-2nV3e9LmSHKCue719kvAz5ihhVM2JGwtU5YKLpCjVE6rXYfkjDMMGhEN6NyEw6Bobz6f6g14dj7Scr9GtyjbqEVMSHe4Ct7Tp8x0skCLucfs0XVpVTQ2Eeder9renuiHbkdcQuFs4Y85UxHvvuYcn8hMLIJgBZ_MoDnPo_demk569AtQIEUTPaOenCK3FxsipW3HeLQOpYVFiQP_sulvJk_MrMSbiAaCCvKMALO29MIhKrYrP8CZEW9a6dNbZNoI_0IM1i5SfhGm_QqMy1Rg0CmjWffJniFB2NZtjn3ecDwX3u3VAI1Of7jlJwfMkqf2ZN5g1djAgD0Yw6iaDzdTm-pVo_1E126QoBm-9oue38401YElM1iMRsx1kMN1CQD054pEDMBIwmcWE=w1365-h946" height="40"> RAFAEL


